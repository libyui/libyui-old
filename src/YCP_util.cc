/****************************************************************************

Copyright (c) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, contact Novell, Inc.

To contact Novell about this file by physical or electronic mail,
you may find current contact information at www.novell.com

****************************************************************************


  File:		YCP_util.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <errno.h>  	// strerror()
#include <iconv.h>

#include <ycp/YCPInteger.h>
#include <ycp/YCPFloat.h>

#define YUILogComponent "ui"
#include <yui/YUILog.h>

#include "YCP_util.h"

using std::string;


std::ostream &
operator<<( std::ostream & stream, const YCPValue & val )
{
    if ( val.isNull() )
	return stream << "<YCPNull>";
    else if ( val->isVoid() )
	return stream << "<nil>";
    else
	return stream << val->toString();
}


bool isNum( const YCPValue & val )
{
    if ( val.isNull() )
	return false;

    return ( val->isInteger() || val->isFloat() );
}


float toFloat( const YCPValue & val )
{
    if ( val->isInteger() )
	return (float) val->asInteger()->value();

    if ( val->isFloat() )
	return val->asFloat()->value();

    yuiError() << "Can't convert " << val << " to float" << std::endl;

    return -1.0;
}


// FIXME: The following code is ugly
// FIXME: The following code is ugly
// FIXME: The following code is ugly

static iconv_t fromUtf8_cd   = (iconv_t)( -1 );
static string  fromUtf8_name = "";

static iconv_t toUtf8_cd     = (iconv_t)( -1 );
static string  toUtf8_name   = "";

static iconv_t fromTo_cd     = (iconv_t)( -1 );
static string  from_name     = "";
static string  to_name	     = "";

static const unsigned recode_buf_size = 1024;
static char	      recode_buf[ recode_buf_size ];

int recode( const string & src,
	    const string & srcEncoding,
	    const string & destEncoding,
	    string & 	   dest )
{
    if ( srcEncoding == destEncoding
	 || src.empty())
    {
	dest = src;
	return 0;
    }

    dest.clear();
    iconv_t cd = (iconv_t)( -1 );

    if ( srcEncoding == "UTF-8" )
    {
	if ( fromUtf8_cd == (iconv_t)( -1 )
	     || fromUtf8_name != destEncoding)
	{
	    if ( fromUtf8_cd != (iconv_t)( -1 ) )
	    {
		iconv_close( fromUtf8_cd );
	    }
	    fromUtf8_cd = iconv_open( destEncoding.c_str(), srcEncoding.c_str() );
	    fromUtf8_name = destEncoding;
	}
	cd = fromUtf8_cd;
    }
    else if ( destEncoding == "UTF-8" )
    {
	if ( toUtf8_cd == (iconv_t)( -1 )
	     || toUtf8_name != srcEncoding)
	{
	    if ( toUtf8_cd != (iconv_t)( -1 ) )
	    {
		iconv_close( toUtf8_cd );
	    }
	    toUtf8_cd = iconv_open( destEncoding.c_str(), srcEncoding.c_str() );
	    toUtf8_name = srcEncoding;
	}
	cd = toUtf8_cd;
    }
    else
    {
	if ( fromTo_cd == (iconv_t)( -1 )
	     || from_name != srcEncoding
	     || to_name != destEncoding)
	{
	    if ( fromTo_cd != (iconv_t)( -1 ) )
	    {
		iconv_close( fromTo_cd );
	    }
	    fromTo_cd = iconv_open( destEncoding.c_str(), srcEncoding.c_str() );
	    from_name = srcEncoding;
	    to_name   = destEncoding;
	}
	cd = fromTo_cd;
    }

    if ( cd == (iconv_t)( -1 ) )
    {
	static bool complained = false;
	if ( ! complained )
	{
	    // glibc-locale is not necessarily installed so only complain once
	    yuiError() << "Recode: (errno " << errno
		       <<  ") -- failed conversion: '" << srcEncoding
		       << "' --> '" << destEncoding << "'"
		       << std::endl;
	    
	    complained = true;
	}
	dest = src;
	return 1;
    }

    size_t inbuf_len  = src.length();
    size_t outbuf_len = recode_buf_size-1;
    char * outbuf = recode_buf;

    char * inptr  = (char *) src.c_str();
    char * outptr = outbuf;
    char * l	  = NULL;

    size_t iconv_ret = (size_t)( -1 );

    do
    {
	iconv_ret = iconv( cd, &inptr, &inbuf_len, &outptr, &outbuf_len );

	if ( iconv_ret == (size_t)( -1 ) )
	{

	    if ( errno == EILSEQ )	// Illegal multibyte sequence?
	    {
		if ( l != outptr )
		{
		    *outptr++ = '?';	// Insert '?' for the illegal character
		    outbuf_len--;	// Account for that '?'
		    l = outptr;
		}
		inptr++;
		continue;
	    }
	    else if ( errno == EINVAL )
	    {
		inptr++;
		continue;
	    }
	    else if ( errno == E2BIG )	// Buffer overflow?
	    {
		*outptr = '\0';		// Terminate converted buffer contents
		dest += recode_buf;	// Append buffer to destination string

		// Set up buffer for the next chunk and start over
		outptr = recode_buf;
		outbuf_len = recode_buf_size-1;
		continue;
	    }
	}

    } while ( inbuf_len != (size_t)(0) );

    *outptr = '\0';		// Terminate converted buffer contents
    dest += recode_buf;		// Append buffer to destination string

    return 0;
}


// EOF

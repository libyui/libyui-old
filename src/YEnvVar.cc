/*
  Copyright (C) 2000-2012 Novell, Inc
  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) version 3.0 of the License. This library
  is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
  License for more details. You should have received a copy of the GNU
  Lesser General Public License along with this library; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301 USA
*/


/*-/

  File:		YEnvVar.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <stdlib.h>	// getenv()
#include <string.h>	// strcmp(), strcasecmp()
#include <ctype.h>	// tolower()

#define YUILogComponent "ui"
#include "YUILog.h"

#include <YEnvVar.h>


YEnvVar::YEnvVar( const std::string & name )
    : _name( name )
    , _isSet( false )
{
    if ( ! _name.empty() )
    {
	const char * val = getenv( _name.c_str() );

	if ( val )
	{
	    _isSet = true;
	    _value = val;
	}
    }
}


bool
YEnvVar::isEqual( const std::string & str, bool caseSensitive ) const
{
    if ( ! _isSet )
	return false;

    if ( caseSensitive )
	return strcmp( _value.c_str(), str.c_str() ) == 0;
    else
	return strcasecmp( _value.c_str(), str.c_str() ) == 0;
}

bool
YEnvVar::contains( const std::string & str, bool caseSensitive ) const
{
    if ( ! _isSet )
	return false;

    if ( caseSensitive )
    {
	return _value.find( str ) != std::string::npos;
    }
    else
    {
	return tolower( _value ).find( tolower( str ) ) != std::string::npos;
    }
}


std::string tolower( const std::string & str )
{
    std::string lowStr;
    lowStr.reserve( str.size() );

    for ( std::string::const_iterator it = str.begin();
	  it != str.end();
	  ++it )
    {
	lowStr += ::tolower( *it );
    }

    return lowStr;
}


std::ostream &
operator<<( std::ostream & stream, const YEnvVar env )
{
    if ( env.name().empty() )
    {
	stream << "<unnamed environment variable>";
    }
    else
    {
	if ( env.isSet() )
	    stream << "$" << env.name() << "=\"" << env.value() << "\"";
	else
	    stream << "$" << env.name() << ": <not set>";
    }

    return stream;
}

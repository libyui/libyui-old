/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:		YCP_util.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCP_util_h
#define YCP_util_h

#include <iostream>

#include <ycp/YCPValue.h>


/**
 * Stream output for YCPValues
 **/
std::ostream & operator<<( std::ostream & stream, const YCPValue & val );

/**
 * Check if a YCPValue is a numerical value (YCPInteger or YCPFloat).
 **/
bool isNum( const YCPValue & val );

/**
 * Convert a numerical YCPValue (YCPInteger or YCPFloat) to float.
 **/
float toFloat( const YCPValue & val );


/**
 * Recode a string from or to UTF-8.
 **/
int recode( const string &	src,
	    const string & 	srcEncoding,
	    const string & 	destEncoding,
	    string & 		dest );



#endif // YCP_util_h

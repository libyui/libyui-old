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

/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/


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

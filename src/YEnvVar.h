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

  File:		YEnvVar.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YEnvVar_h
#define YEnvVar_h

#include <string>
#include <iosfwd>

using std::string;


/**
 * Helper class to represent an environment variable and its value.
 **/
class YEnvVar
{
public:
    /**
     * Constructor:
     * Retrieve the environment variable 'name' and store the value
     * (unless 'name' is empty).
     **/
    YEnvVar( const string & name = string() );

    /**
     * Return the name of the environment variable.
     **/
    string name() const { return _name; }

    /**
     * Return 'true' if the environment variable is set.
     **/
    bool isSet() const { return _isSet; }

    /**
     * Return the value of the environment variable.
     **/
    string value() const { return _value; }

    /**
     * Return 'true' if the environment variable is set and the value is
     * 'str'. 
     **/
    bool isEqual( const string & str, bool caseSensitive = false ) const;

    /**
     * Case-insensitive comparison (shortcut for isEqual() ):
     * Return 'true' if the environment variable is set and the value is
     * 'str'. 
     **/
    bool operator==( const string & str ) const
	{ return isEqual( str ); }
    
    /**
     * Return 'true' if the environment variable is set and the value contains
     * 'str'.
     **/
    bool contains( const string & str, bool caseSensitive = false ) const;

    
private:

    string _name;
    string _value;
    bool   _isSet;
};


/**
 * Stream output for YEnvVar
 **/
std::ostream & operator<<( std::ostream & stream, const YEnvVar env );


/**
 * Return 'str' converted to lower case.
 **/
string tolower( const string & str );


#endif // YEnvVar_h

/*
  |****************************************************************************
  |
  | Copyright (c) 2000 - 2012 Novell, Inc.
  | All Rights Reserved.
  |
  | This program is free software; you can redistribute it and/or
  | modify it under the terms of version 2 of the GNU General Public License as
  | published by the Free Software Foundation.
  |
  | This program is distributed in the hope that it will be useful,
  | but WITHOUT ANY WARRANTY; without even the implied warranty of
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
  | GNU General Public License for more details.
  |
  | You should have received a copy of the GNU General Public License
  | along with this program; if not, contact Novell, Inc.
  |
  | To contact Novell about this file by physical or electronic mail,
  | you may find current contact information at www.novell.com
  |
  |****************************************************************************
*/



 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////   __/\\\\\\_____________/\\\__________/\\\________/\\\___/\\\________/\\\___/\\\\\\\\\\\_           ////
 ////    _\////\\\____________\/\\\_________\///\\\____/\\\/___\/\\\_______\/\\\__\/////\\\///__          ////
 ////     ____\/\\\______/\\\__\/\\\___________\///\\\/\\\/_____\/\\\_______\/\\\______\/\\\_____         ////
 ////      ____\/\\\_____\///___\/\\\_____________\///\\\/_______\/\\\_______\/\\\______\/\\\_____        ////
 ////       ____\/\\\______/\\\__\/\\\\\\\\\_________\/\\\________\/\\\_______\/\\\______\/\\\_____       ////
 ////        ____\/\\\_____\/\\\__\/\\\////\\\________\/\\\________\/\\\_______\/\\\______\/\\\_____      ////
 ////         ____\/\\\_____\/\\\__\/\\\__\/\\\________\/\\\________\//\\\______/\\\_______\/\\\_____     ////
 ////          __/\\\\\\\\\__\/\\\__\/\\\\\\\\\_________\/\\\_________\///\\\\\\\\\/_____/\\\\\\\\\\\_    ////
 ////           _\/////////___\///___\/////////__________\///____________\/////////______\///////////__   ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                 widget abstraction library providing Qt, GTK and ncurses frontends                  ////
 ////                                                                                                     ////
 ////                                   3 UIs for the price of one code                                   ////
 ////                                                                                                     ////
 ////                                       ***  core library  ***                                        ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                              (C) SUSE Linux GmbH    ////
 ////                                                                                                     ////
 ////                                                              libYUI-AsciiArt (C) 2012 Björn Esser   ////
 ////                                                                                                     ////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-/

  File:		YEnvVar.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YEnvVar_h
#define YEnvVar_h

#include <string>
#include <iosfwd>



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
    YEnvVar( const std::string & name = std::string() );

    /**
     * Return the name of the environment variable.
     **/
    std::string name() const { return _name; }

    /**
     * Return 'true' if the environment variable is set.
     **/
    bool isSet() const { return _isSet; }

    /**
     * Return the value of the environment variable.
     **/
    std::string value() const { return _value; }

    /**
     * Return 'true' if the environment variable is set and the value is
     * 'str'.
     **/
    bool isEqual( const std::string & str, bool caseSensitive = false ) const;

    /**
     * Case-insensitive comparison (shortcut for isEqual() ):
     * Return 'true' if the environment variable is set and the value is
     * 'str'.
     **/
    bool operator==( const std::string & str ) const
	{ return isEqual( str ); }

    /**
     * Return 'true' if the environment variable is set and the value contains
     * 'str'.
     **/
    bool contains( const std::string & str, bool caseSensitive = false ) const;


private:

    std::string	_name;
    std::string	_value;
    bool	_isSet;
};


/**
 * Stream output for YEnvVar
 **/
std::ostream & operator<<( std::ostream & stream, const YEnvVar env );


/**
 * Return 'str' converted to lower case.
 **/
std::string tolower( const std::string & str );


#endif // YEnvVar_h

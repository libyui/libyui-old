/*************************************************************************************************************

 Copyright (C) 2000 - 2010 Novell, Inc.   All Rights Reserved.

 This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 Public License as published by the Free Software Foundation; either version 2 of the License, or (at your
 option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License along with this program; if not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*************************************************************************************************************/



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

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

  File:		YUIException.cc

		Stolen from zypp/libzypp/base/Exception.cc

  Author:	Michael Andres    <ma@suse.de>
  Maintainer:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <sstream>
#include <string.h>	// strerror()
#include <stdio.h>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUIException.h"
#include "YWidget.h"




std::string YCodeLocation::asString() const
{
    std::string str( _file );
    str += "(" + _func + "):";

    char formatted_number[ 20 ];
    sprintf( formatted_number, "%u", _line );

    str += formatted_number;

    return str;
}


std::ostream &
operator<<( std::ostream & str, const YCodeLocation & obj )
{
    return str << obj.asString();
}


YUIException::YUIException()
{
    // NOP
}

YUIException::YUIException( const std::string & msg_r )
    : _msg( msg_r )
{
    // NOP
}


YUIException::~YUIException() throw()
{
    // NOP
}


std::string
YUIException::asString() const
{
    std::ostringstream str;
    dumpOn( str );
    return str.str();
}


std::ostream &
YUIException::dumpOn( std::ostream & str ) const
{
    return str << _msg;
}


std::ostream &
YUIException::dumpError( std::ostream & str ) const
{
    return dumpOn( str << _where << ": " );
}


std::ostream &
operator<<( std::ostream & str, const YUIException & obj )
{
    return obj.dumpError( str );
}


std::string
YUIException::strErrno( int errno_r )
{
    return strerror( errno_r );
}


std::string
YUIException::strErrno( int errno_r, const std::string & msg )
{
    std::string ret( msg );
    ret += ": ";
    return ret += strErrno( errno_r );
}


void
YUIException::log( const YUIException & 	exception,
		   const YCodeLocation & 	location,
		   const char * const 		prefix )
{
    YUILog::warning( YUILogComponent,
		     location.file().c_str(),
		     location.line(),
		     location.func().c_str() )
			 << "\t" << prefix << " " << exception.asString() << std::endl;
}


std::ostream &
YUIUnknownPropertyException::dumpOn( std::ostream & str ) const
{
    if ( widget() )
    {
	return str << widget()->widgetClass()
		   << " has no property named \""
		   << property().name()
		   << "\""
		   << std::endl;
    }
    else
    {
	return str << "Unknown property name \""
		   << property().name()
		   << "\""
		   << std::endl;
    }
}


std::ostream &
YUIPropertyTypeMismatchException::dumpOn( std::ostream & str ) const
{
    std::string widgetClass;

    if ( widget() )
	widgetClass = std::string( widget()->widgetClass() ) + "::";

    return str << "Property type mismatch: "
	       << widgetClass
	       << property().name()
	       << " is type "
	       << property().typeAsStr()
	       << ", not "
	       << YProperty::typeAsStr( type() )
	       << std::endl;
}


std::ostream &
YUISetReadOnlyPropertyException::dumpOn( std::ostream & str ) const
{
    std::string widgetClass;

    if ( widget() )
	widgetClass = std::string( widget()->widgetClass() ) + "::";

    return str << "Property "
	       << widgetClass
	       << property().name()
	       << "is read-only!"
	       << std::endl;
}


std::ostream &
YUIBadPropertyArgException::dumpOn( std::ostream & str ) const
{
    std::string widgetClass;

    if ( widget() )
	widgetClass = std::string( widget()->widgetClass() ) + "::";

    return str << "Bad argument for property "
	       << widgetClass
	       << property().name()
	       << ": "
	       << msg()
	       << std::endl;
}

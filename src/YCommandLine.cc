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

  File:		YCommandLine.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <stdlib.h>	// malloc()
#include <string.h>	// strdup()

#include <vector>
#include <fstream>

#include "YCommandLine.h"
#include "YUIException.h"

#define YUILogComponent "ui"
#include "YUILog.h"


struct YCommandLinePrivate
{
    std::vector<std::string> args;
};





YCommandLine::YCommandLine()
    : priv( new YCommandLinePrivate() )
{
    YUI_CHECK_NEW( priv );

    std::ifstream cmdline( "/proc/self/cmdline", std::ifstream::in | std::ifstream::binary );

    while ( cmdline.good() )
    {
	std::string arg;
	getline( cmdline, arg, '\0' );

	if ( ! arg.empty() )
	{
	    yuiDebug() << "Arg #" << priv->args.size()
		       << ": \"" << arg << "\"" << std::endl;

	    priv->args.push_back( arg );
	}
    }
}


YCommandLine::~YCommandLine()
{

}


int
YCommandLine::argc() const
{
    return priv->args.size();
}


char **
YCommandLine::argv() const
{
    char ** argArray = (char **) ( malloc( argc() * sizeof( char * ) ) );

    if ( argArray )
    {
	for ( int i=0; i < argc(); i++ )
	{
	    argArray[ i ] = strdup( priv->args[i].c_str() );
	}
    }

    return argArray;
}


void
YCommandLine::add( const std::string & arg )
{
    priv->args.push_back( arg );
}


std::string
YCommandLine::arg( int index ) const
{
    YUI_CHECK_INDEX( index, 0, (int) priv->args.size()-1 );

    return priv->args[ index ];
}


void
YCommandLine::remove( int index )
{
    YUI_CHECK_INDEX( index, 0, (int) priv->args.size()-1 );

    priv->args.erase( priv->args.begin() + index );
}


void
YCommandLine::replace( int index, const std::string & newArg )
{
    YUI_CHECK_INDEX( index, 0, (int) priv->args.size()-1 );

    priv->args[ index ] = newArg;
}


int
YCommandLine::find( const std::string & argName ) const
{
    for ( int i=0; i < argc(); i++ )
    {
	if ( priv->args[i] == argName )
	    return i;
    }

    return -1;
}

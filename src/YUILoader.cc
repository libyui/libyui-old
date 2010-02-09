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

  File:		YUILoader.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <stdlib.h>		// getenv()
#include <unistd.h>		// isatty()

#include "YUILoader.h"
#include "YUIPlugin.h"
#include "YUIException.h"


void YUILoader::loadUI( bool withThreads )
{
    const char * envDisplay = getenv( "DISPLAY" );

    if ( envDisplay )
    {
	//
	// Qt UI
	//
	
	try
	{
	    loadPlugin( YUIPlugin_Qt, withThreads );
	    return;
	}
	catch ( YUIException & ex)
	{
	    YUI_CAUGHT( ex );
	}
    }

    if ( isatty( STDOUT_FILENO ) )
    {
	//
	// NCurses UI
	//
	
	try
	{
	    loadPlugin( YUIPlugin_NCurses, withThreads );
	    return;
	}
	catch ( YUIException & ex)
	{
	    YUI_CAUGHT( ex );
	    YUI_RETHROW( ex ); // what else to do here?
	}
    }
    else
    {
	YUI_THROW( YUICantLoadAnyUIException() );
    }
}


void YUILoader::loadPlugin( const string & name, bool withThreads )
{
    YUIPlugin uiPlugin( name.c_str() );

    if ( uiPlugin.success() )
    {
	createUIFunction_t createUI = (createUIFunction_t) uiPlugin.locateSymbol( "_Z8createUIb" ); // createUI(bool)

	if ( createUI )
	{
	    YUI * ui = createUI( withThreads ); // no threads

	    if ( ui )
		return;
	}
    }

    YUI_THROW( YUIPluginException( name ) );
}

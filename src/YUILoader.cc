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

  File:		YUILoader.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <stdlib.h>		// getenv()
#include <unistd.h>		// isatty()a
#include <sys/stat.h>
#include <string.h>

#include "YCommandLine.h"
#include "YUILoader.h"
#include "YUIPlugin.h"
#include "YUIException.h"
#include "YPath.h"

#include "Libyui_config.h"


void YUILoader::loadUI( bool withThreads )
{
    const char * envDisplay = getenv( "DISPLAY" );

    YCommandLine cmdline;

    bool wantNcurses = cmdline.find("--ncurses") != -1;
    bool wantQt = cmdline.find("--qt") != -1;
    bool wantGtk = cmdline.find("--gtk") != -1;

    bool haveQt = pluginExists( YUIPlugin_Qt );
    bool haveGtk = pluginExists( YUIPlugin_Gtk );

    if ( envDisplay && !wantNcurses )
    {
	std::string wantedGUI;

	if ( haveQt && !wantGtk)
	   wantedGUI = YUIPlugin_Qt;
	else if ( haveGtk && !wantQt )
	   wantedGUI = YUIPlugin_Gtk;

	if ( strcmp( wantedGUI.c_str(), "" ) )
	{
	   try
	   {
	      loadPlugin( wantedGUI, withThreads );
	      return;
	   }
	   catch ( YUIException & ex)
	   {
	      YUI_CAUGHT( ex );
	   }
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


void YUILoader::loadPlugin( const std::string & name, bool withThreads )
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

bool YUILoader::pluginExists( const std::string & pluginBaseName )
{
    struct stat fileinfo;
    std::string pluginName = PLUGIN_PREFIX;

    pluginName.append( pluginBaseName );
    pluginName.append( PLUGIN_SUFFIX );

    YPath plugin ( PLUGINDIR, pluginName );

    return stat( plugin.path().c_str(), &fileinfo) == 0;

}

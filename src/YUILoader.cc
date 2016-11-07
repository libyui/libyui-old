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

#define YUILogComponent "ui"
#include "YUILog.h"
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

void YUILoader::deleteUI()
{
    if ( YUI::_ui )
    {
        yuiMilestone() << "Shutting down UI" << std::endl;
        delete YUI::_ui;

        YUI::_ui = 0;
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

            // At this point the concrete UI will have loaded its own
            // internal plugins and registered their destructors.
            // Our destructor must get called before those get dlclose'd.
            //
            // Formerly ~YUI was called quite late, which called ~YQUI
            // and that ran code in the already unloaded Qt internal plugins.
            atexit(deleteUI);

	    if ( ui )
		return;
	}
    }

    YUI_THROW( YUIPluginException( name ) );
}

void YUILoader::loadExternalWidgetsPlugin ( const std::string& name, const std::string& plugin_name, const std::string& symbol )
{
  YUIPlugin uiPlugin ( plugin_name.c_str() );

  if ( uiPlugin.success() )
  {
    createEWFunction_t createEW = ( createEWFunction_t ) uiPlugin.locateSymbol ( symbol.c_str() );

    if ( createEW )
    {
      YExternalWidgets * we = createEW ( name.c_str() );

      if ( we )
        return;
    }
  }

  YUI_THROW ( YUIPluginException ( plugin_name ) );
}

void YUILoader::loadExternalWidgets ( const std::string& name, const std::string& symbol )
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
        std::string wantedGUI = name;
        wantedGUI.append("-");

        if ( haveQt && !wantGtk)
           wantedGUI.append(YUIPlugin_Qt);
        else if ( haveGtk && !wantQt )
           wantedGUI.append(YUIPlugin_Gtk);

        try
        {
            loadExternalWidgetsPlugin(name, wantedGUI, symbol );
            return;
        }
        catch ( YUIException & ex)
        {
            YUI_CAUGHT( ex );
        }
    }

    //
    // NCurses UI (test on tty has already been done by loadUI)
    //

    try
    {
        std::string wantedNcurses = name;
        wantedNcurses.append("-");
        wantedNcurses.append(YUIPlugin_NCurses);
        loadExternalWidgetsPlugin(name, wantedNcurses, symbol );
        return;
    }
    catch ( YUIException & ex)
    {
        YUI_CAUGHT( ex );
        YUI_RETHROW( ex ); // what else to do here?
    }
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

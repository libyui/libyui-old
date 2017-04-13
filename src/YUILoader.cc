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
#include "YSettings.h"

#include "Libyui_config.h"


void YUILoader::loadUI( bool withThreads )
{
    bool isGtk = false;
    const char * envDisplay = getenv( "DISPLAY" );
    const char * envDesktop = getenv( "XDG_CURRENT_DESKTOP" );
    std::string wantedGUI;

    yuiMilestone () << "XDG_CURRENT_DESKTOP: \"" << envDesktop << "\"" << std::endl;

    // Taken from: https://specifications.freedesktop.org/menu-spec/menu-spec-1.1.html#onlyshowin-registry
    isGtk = ( ( strstr( envDesktop, "Cinnamon" ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "GNOME"    ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "LXDE"     ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "MATE"     ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "Pantheon" ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "ROX"      ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "Unity"    ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "XFCE"     ) != NULL ) || isGtk );

    if( isGtk )
    {
	yuiMilestone () << "Detected a Gtk-based desktop environment." << std::endl;
	yuiMilestone () << "Prefering Gtk-UI if available." << std::endl;
    }

    YCommandLine cmdline;

    bool wantNcurses = cmdline.find("--ncurses") != -1;
    if( wantNcurses )
	yuiMilestone () << "Using UI-backend: \"" << YUIPlugin_NCurses << "\". Forced on command-line." << std::endl;
    bool wantQt = cmdline.find("--qt") != -1;
    if( wantQt )
	yuiMilestone () << "Using UI-backend: \"" << YUIPlugin_Qt << "\". Forced on command-line." << std::endl;
    bool wantGtk = cmdline.find("--gtk") != -1;
    if( wantGtk )
	yuiMilestone () << "Using UI-backend: \"" << YUIPlugin_Gtk << "\". Forced on command-line." << std::endl;

    bool haveQt = pluginExists( YUIPlugin_Qt );
    bool haveGtk = pluginExists( YUIPlugin_Gtk );

    if ( envDisplay && !wantNcurses )
    {
	// Qt is default if available.
	if ( haveQt )
	   wantedGUI = YUIPlugin_Qt;

	// Do we want to use Gtk instead?
	if ( haveGtk && ( isGtk || wantGtk ) && !wantQt )
	   wantedGUI = YUIPlugin_Gtk;

	if ( strcmp( wantedGUI.c_str(), "" ) )
	{
	   yuiMilestone () << "Using UI-backend: \"" << wantedGUI << "\""<< std::endl;
	   try
	   {
	      YSettings::loadedUI( wantedGUI, true );
	      loadPlugin( wantedGUI, withThreads );
	      return;
	   }
	   catch ( YUIException & ex )
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

	wantedGUI = YUIPlugin_NCurses;
	yuiMilestone () << "Using UI-backend: \"" << wantedGUI << "\""<< std::endl;

	try
	{
	    YSettings::loadedUI( wantedGUI, true );
	    loadPlugin( wantedGUI, withThreads );
	    return;
	}
	catch ( YUIException & ex )
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
    std::string wantedGUI = name;
    wantedGUI.append( "-" );
    wantedGUI.append( YSettings::loadedUI() );

    try
    {
        loadExternalWidgetsPlugin(name, wantedGUI, symbol );
        return;
    }
    catch ( YUIException & ex )
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

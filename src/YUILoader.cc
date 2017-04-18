/*
  Copyright (C) 2000-2017 Novell, Inc
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
    const char * envDesktop = getenv( "XDG_CURRENT_DESKTOP" )  ?: "";
    const char * envDisplay = getenv( "DISPLAY" )              ?: "";
    const char * envPreset  = getenv( "YUI_PREFERED_BACKEND" ) ?: "";
    std::string wantedGUI;

    yuiMilestone () << "DISPLAY: \""              << envDisplay << "\"" << std::endl;
    yuiMilestone () << "XDG_CURRENT_DESKTOP: \""  << envDesktop << "\"" << std::endl;
    yuiMilestone () << "YUI_PREFERED_BACKEND: \"" << envPreset  << "\"" << std::endl;

    // Taken from: https://specifications.freedesktop.org/menu-spec/menu-spec-1.1.html#onlyshowin-registry
    isGtk = ( ( strstr( envDesktop, "Cinnamon" ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "GNOME"    ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "LXDE"     ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "MATE"     ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "Pantheon" ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "ROX"      ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "Unity"    ) != NULL ) || isGtk );
    isGtk = ( ( strstr( envDesktop, "XFCE"     ) != NULL ) || isGtk );

    if( isGtk ) yuiMilestone () << "Detected a Gtk-based desktop environment." << std::endl
                                << "Prefering Gtk-UI if available and no" << std::endl
                                << "user-selected override is present." << std::endl;

    YCommandLine cmdline;

    bool wantGtk      = ( cmdline.find( "--gtk" )     != -1 );
    bool wantNcurses  = ( cmdline.find( "--ncurses" ) != -1 );
    bool wantQt       = ( cmdline.find( "--qt" )      != -1 );
    bool haveUIPreset = ( wantGtk || wantNcurses || wantQt );

    if( !haveUIPreset )
    {
	wantGtk     = ( strcmp( envPreset, YUIPlugin_Gtk )     == 0 );
	wantNcurses = ( strcmp( envPreset, YUIPlugin_NCurses ) == 0 );
	wantQt      = ( strcmp( envPreset, YUIPlugin_Qt )      == 0 );
    }

    if( wantGtk )     wantedGUI = YUIPlugin_Gtk;
    if( wantNcurses ) wantedGUI = YUIPlugin_NCurses;
    if( wantQt )      wantedGUI = YUIPlugin_Qt;

    yuiMilestone () << "User-selected UI-plugin: \"" << wantedGUI << "\"" << std::endl;

    bool haveGtk     = pluginExists( YUIPlugin_Gtk );
    bool haveNcurses = pluginExists( YUIPlugin_NCurses );
    bool haveQt      = pluginExists( YUIPlugin_Qt );

    // This reset is intentional, so the loader can work it's magic
    // selecting an UI-plugin as described in the documentation.
    wantedGUI="";

    // Set the UI-Plugin
    if ( ( haveGtk || haveQt ) && strcmp ( envDisplay, "" ) &&
	 ( !wantNcurses || !isatty( STDOUT_FILENO ) ) )
    {
	// Qt is default if available.
	if ( haveQt )
	    wantedGUI = YUIPlugin_Qt;

	// Do we want to use Gtk instead?
	if ( haveGtk && ( ( ( isGtk || wantGtk ) && !wantQt ) || !haveQt ) )
	    wantedGUI = YUIPlugin_Gtk;
    }

    else if ( haveNcurses && isatty( STDOUT_FILENO ) )
    {
	// We use NCurses.
	wantedGUI = YUIPlugin_NCurses;
    }

    // Load the wanted UI-plugin.
    if( wantedGUI != "" )
    {
	yuiMilestone () << "Using UI-plugin: \"" << wantedGUI << "\""<< std::endl;
	YSettings::loadedUI( wantedGUI, true );

	try
	{
	    loadPlugin( wantedGUI, withThreads );
	    return;
	}

	catch ( YUIException & ex )
	{
	    YUI_CAUGHT( ex );

	    // Default to NCurses, if possible.
	    if( wantedGUI != YUIPlugin_NCurses && haveNcurses && isatty( STDOUT_FILENO ) )
	    {
		yuiWarning () << "Defaulting to: \"" << YUIPlugin_NCurses << "\""<< std::endl;
		YSettings::loadedUI( YUIPlugin_NCurses, true );

		try
		{
		    loadPlugin( YUIPlugin_NCurses, withThreads );
		    return;
		}

		catch ( YUIException & ex )
		{
		    YUI_CAUGHT( ex );
		}
	    }

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

    bool haveExternal = pluginExists( wantedGUI );

    if( haveExternal )
    {
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

    else
    {
	YUI_THROW( YUICantLoadAnyUIException() );
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

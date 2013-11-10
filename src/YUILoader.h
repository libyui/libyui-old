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

  File:		YUILoader.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YUILoader_h
#define YUILoader_h


#include <string>

#include "YUI.h"
#include "YExternalWidgets.h"



#define YUIPlugin_Qt		"qt"
#define YUIPlugin_NCurses	"ncurses"
#define YUIPlugin_Gtk		"gtk"


/**
 * Class to load one of the concrete UI plug-ins: Qt, NCurses, Gtk.
 **/
class YUILoader
{
public:
    /**
     * Load any of the available UI plug-ins in this order:
     * - Qt      if $DISPLAY is set
     * - NCurses if stdout is a tty
     **/
    static void loadUI( bool withThreads = false );

    /**
     * Load a UI plug-in. 'name' is one of the YUIPlugin_ -defines above.
     *
     * This might throw exceptions.
     **/
    static void loadPlugin( const std::string & name, bool withThreads = false );

    static bool pluginExists( const std::string & pluginBaseName );
    
    /**
     * Load the given External Widgets plugin followed by its graphical extension implementation 
     * in the following order in the same way as loadUI:
     * - Qt, Gtk or NCurses 
     * 
     * 'name'   is the user defined plugin name, graphical extension implementations have to 
     *          be called 'name'-qt, 'name'-gtk and 'name'-ncurses. Following this rule plugin
     *          file names are as libyui-XX-YY.so.VER where:
     *               XX  is the user defined name
     *               YY  is the UI used (ncurses, gtk, qt)
     *               VER is the libyui so version
     * 'symbol' is the function symbol to be loaded, e.g. YExternalWidgets* 'symbol'(void)
     *          usually YExternalWidgets* createWE(void) see createEWFunction_t
     **/
    static void loadExternalWidgets( const std::string & name, const std::string & symbol="_Z8createWEv" );
    
private:
    YUILoader()  {}
    ~YUILoader() {}
    
    /**
     * Used by loadExternalWidgets to load the graphical plugin specialization.
     * 
     * This might throw exceptions.
     **/
    static void loadExternalWidgetsPlugin( const std::string & name, const std::string & symbol );
};


/**
 * Every UI plug-in has to provide a function
 *
 *     YUI * createUI( bool withThreads )
 *
 * that creates a UI of that specific type upon the first call and returns that
 * singleton for all subsequent calls.
 **/
typedef YUI * (*createUIFunction_t)( bool );

/**
 * Every WE extension plug-in has to provide a function
 *
 *     YExternalWidgets * createWE( )
 *
 * that creates a WE of that specific type upon the first call and returns that
 * singleton for all subsequent calls.
 **/
typedef YExternalWidgets * (*createEWFunction_t)( void );

#endif // YUILoader_h

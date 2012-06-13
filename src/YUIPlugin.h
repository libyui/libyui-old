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

  File:		YUIPlugin.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YUIPlugin_h
#define YUIPlugin_h

#include <string>


/**
 * Wrapper class for dlopen() and related.
 **/
class YUIPlugin
{
public:

    /**
     * Constructor: Load the specified plugin library
     * from the standard UI plugin directory (/usr/lib/yui/).
     **/
    YUIPlugin( const char * pluginLibBaseName );

    /**
     * Destructor.
     *
     * Please note that this will NOT attempt to unload the plugin library
     * since this is usually counterproductive. If unloading the plugin is
     * desired, call unload() manually.
     **/
    virtual ~YUIPlugin();

    /**
     * Unload this plugin. This calls dlclose() which will unload the plugin
     * library if it is no longer used, i.e. if the reference count dlopen()
     * uses reaches 0.
     **/
    void unload();

    /**
     * Try to locate the specified symbol (function or global variable) in the
     * plugin library.
     *
     * Returns the in-memory address of that symbol or 0 if it could not be
     * found or if loading the plugin library had failed in the constructor.
     **/
    void * locateSymbol( const char * symbol );

    /**
     * Returns 'true' if there was an error loading the plugin.
     **/
    bool error() const;

    /**
     * Returns 'true' if there was no error loading the plugin.
     **/
    bool success() const;

    /**
     * Returns a human readable (but in most cases untranslated) error message
     * if there was an error.
     **/
    std::string errorMsg() const;

protected:

    /**
     * Returns the dlopen() handle of the plugin library.
     **/
    void * pluginLibHandle() { return _pluginLibHandle; }

    /**
     * Returns the base name of the plugin library.
     **/
    std::string pluginLibBaseName() const { return _pluginLibBaseName; }

    /**
     * Returns the full path of the plugin library.
     **/
    std::string pluginLibFullPath() const;

    /**
     * Loads the plugin library.
     **/
    void loadPluginLib();

private:

    std::string	_pluginLibBaseName;
    void * 	_pluginLibHandle;
    std::string	_errorMsg;
};


#endif // YUIPlugin_h

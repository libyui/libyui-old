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

  File:		YUIPlugin.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YUIPlugin_h
#define YUIPlugin_h

#define PLUGIN_PREFIX "libpy2"
#define PLUGIN_SUFFIX ".so.2"

#include <string>
using std::string;


/**
 * Wrapper class for dlopen() and related.
 **/
class YUIPlugin
{
public:

    /**
     * Constructor: Load the specified plugin library
     * from the standard UI plugin directory (/usr/lib/YaST2/plugin).
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
    string errorMsg() const;

protected:

    /**
     * Returns the dlopen() handle of the plugin library.
     **/
    void * pluginLibHandle() { return _pluginLibHandle; }

    /**
     * Returns the base name of the plugin library.
     **/
    string pluginLibBaseName() const { return _pluginLibBaseName; }

    /**
     * Returns the full path of the plugin library.
     **/
    string pluginLibFullPath() const;

    /**
     * Loads the plugin library.
     **/
    void loadPluginLib();

private:

    string	_pluginLibBaseName;
    void * 	_pluginLibHandle;
    string	_errorMsg;
};


#endif // YUIPlugin_h

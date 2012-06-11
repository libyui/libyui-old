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

  File:		YUIPlugin.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <dlfcn.h>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUIPlugin.h"
#include "YPath.h"

#include "config.h"


YUIPlugin::YUIPlugin( const char * pluginLibBaseName )
{
    _pluginLibBaseName = std::string( pluginLibBaseName );

    std::string pluginFilename = pluginLibFullPath();

    _pluginLibHandle = dlopen( pluginFilename.c_str(),
			       RTLD_NOW | RTLD_GLOBAL);

    if ( ! _pluginLibHandle )
    {
	_errorMsg = dlerror();

	yuiError() << "Could not load UI plugin \"" << pluginLibBaseName
		   << "\": " << _errorMsg
		   << std::endl;
    }
}


YUIPlugin::~YUIPlugin()
{
    // This intentionally does NOT call unload(): This would be
    // counterproductive for almost all use cases of this class.
}


void
YUIPlugin::unload()
{
    if ( _pluginLibHandle )
	dlclose( _pluginLibHandle );
}


std::string
YUIPlugin::pluginLibFullPath() const
{

    std::string pluginName = PLUGIN_PREFIX;
    pluginName.append( _pluginLibBaseName );
    pluginName.append( PLUGIN_SUFFIX );

    YPath plugin( PLUGINDIR, pluginName );

    return plugin.path();
}


void * YUIPlugin::locateSymbol( const char * symbol )
{
    if ( ! _pluginLibHandle )
	return 0;

    void * addr = dlsym( _pluginLibHandle, symbol );

    if ( ! addr )
    {
	yuiError() << "Could not locate symbol \"" << symbol
		   << "\" in " << pluginLibFullPath()
		   << std::endl;
    }

    return addr;
}


bool YUIPlugin::error() const
{
    return _pluginLibHandle == 0;
}


bool YUIPlugin::success() const
{
    return _pluginLibHandle != 0;
}


std::string YUIPlugin::errorMsg() const
{
    return _errorMsg;
}

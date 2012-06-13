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

  File:         YIconLoader.cc

  Author:       Katarína Machálková <kmachalkova@novell.com>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include <sys/stat.h>
#include <sstream>

#include "YIconLoader.h"

#define FALLBACK_ICON_PATH "/usr/share/icons/hicolor/"

YIconLoader::YIconLoader()
{
    addIconSearchPath(FALLBACK_ICON_PATH);
}

YIconLoader::~YIconLoader()
{
}

void YIconLoader::setIconBasePath( string path)
{
    _iconBasePath = path;
}

string YIconLoader::iconBasePath() const
{
    return _iconBasePath;
}

void YIconLoader::addIconSearchPath( string path )
{
    icon_dirs.push_front( path );
}

string YIconLoader::findIcon( string name )
{
    // No extension -> add some
    string::size_type loc = name.find(".png");
    if ( loc == string::npos )
	name += ".png";

    // Absolute path -> return it
    if (name[0] == '/')
        return name;

    string fullPath;

    // Look in global search path
    if ( !_iconBasePath.empty () )
    {
	fullPath = _iconBasePath + name;
        if ( fileExists ( fullPath ) )
	{
	    yuiMilestone() << "Found " << name << " in global search path" << endl;
	    return fullPath;
	}
    }

    // Now search the fallback dirs
    list<string>::iterator listIt = icon_dirs.begin();

    while( listIt != icon_dirs.end() )
    {
	// Something like relative path
	if ( name.find('/') != string::npos )
            fullPath = *listIt +  name;
	// No '/' chars, just the name -> use '22x22/apps' fallback
	else
	   fullPath = *listIt + "22x22/apps/" + name;

        if ( fileExists( fullPath ) )
	{
	    yuiMilestone() << "Found " << name << " in " <<  *listIt << " search path" << endl;
	    return fullPath;
	}

	yuiMilestone() <<  name << " not found in " <<  *listIt << " search path, skipping" << endl;
        listIt++;
    }

    return "";
}

bool YIconLoader::fileExists( string fname )
{
    struct stat fileInfo;
    int ret = stat (fname.c_str(), &fileInfo);

    return ( ret == 0 );
}

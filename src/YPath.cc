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

  File:		YPath.cc

  Author:	Björn Esser <bjoern.esser@gmail.com>

/-*/

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <dirent.h>
#include <vector>

#include "YPath.h"
#include "YSettings.h"
#include "config.h"

#define YUILogComponent "ui"
#include "YUILog.h"

YPath::YPath ( string directory, string filename )
{
  yuiMilestone () << "Looking for " << filename << endl;

  bool isThemeDir = ! directory.compare ( THEMEDIR );
  string progSubDir;
  string themeSubDir = "";
  vector<string> dirList;
  vector<string> fullList;

  progSubDir = YSettings::access () -> getProgSubDir ();

  yuiMilestone () << "Preferring subdir: " << progSubDir << endl;

  if ( isThemeDir )
  {
    themeSubDir = "/current";
    dirList.push_back ( directory + "/" + progSubDir + themeSubDir );
    dirList.push_back ( directory + themeSubDir );
  }

  dirList.push_back ( directory + "/" + progSubDir );
  dirList.push_back ( directory );

  for ( vector<string>::iterator x = dirList.begin () ; x != dirList.end () && fullPath.compare ( "" ) == 0 ; ++x )
  {
    yuiMilestone () << "scanning subdir: " << *x << endl;

    vector<string> fileList = lsDir( *x );
    fullList.push_back( *x );

    for ( vector<string>::iterator i = fileList.begin () ; i != fileList.end () && fullPath.compare ( "" ) == 0 ; ++i )
    {
      if ( *i != "." && *i != ".." )		// filter out parent and curdir
      {
	stringstream fullname;
	fullname << directory << "/" << ( *i );

	if ( *i == filename )
	  fullPath = fullname.str ();
	else
	{
	  fullList.push_back ( fullname.str () );
	  fullPath = lookRecursive ( fullname.str (), filename, fullList );
	}
      }
    }
  }

  if( fullPath.compare ( "" ) != 0 )
    yuiMilestone() << "Found " << filename << " in " << dir() << endl;
  else
    yuiMilestone() << "Could NOT find " << filename << " by looking recursive inside " << directory << endl;
}

YPath::~YPath()
{
}

vector<string> YPath::lsDir( string directory )
{
  vector<string> fileList;
  DIR *dir;
  struct dirent *ent;

  if ( ( dir = opendir( directory.c_str () ) ) != NULL )
  {
    yuiMilestone() << "Looking in " << directory << endl;

    while ( ( ent = readdir ( dir ) ) != NULL )
      fileList.push_back ( ent -> d_name );

    closedir ( dir );
  }
  else
    yuiMilestone() << "\"" << directory << "\" does NOT exist." << endl;

  return fileList;
}

string YPath::lookRecursive( string directory, string filename, vector<string> fullList )
{
  vector<string> fileList = lsDir( directory );
  string file = "";

  for ( vector<string>::iterator i = fileList.begin() ; i != fileList.end() && file.compare ( "" ) == 0 ; ++i )
  {
    if ( *i != "." && *i != ".." )            // filter out parent and curdir
    {
      stringstream fullname;
      fullname << directory << "/" << ( *i );

      if ( *i == filename )
	file = fullname.str ();
      else
      {
	fullList.push_back ( fullname.str() );
	file = lookRecursive ( fullname.str(), filename, fullList );
      }
    }
  }
  return file;
}

string YPath::path()
{
  return fullPath;
}

string YPath::dir()
{
  return fullPath.substr ( 0, fullPath.rfind( "/" ) );
}

/*************************************************************************************************************

 Copyright (C) 2012 Björn Esser

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

YPath::YPath ( const std::string & directory, const std::string & filename )
{
  yuiMilestone () << "Looking for " << filename << std::endl;

  bool				isThemeDir = ! directory.compare ( THEMEDIR );
  std::string			progSubDir = YSettings::access () -> getProgSubDir ();
  std::string			filenameNoPrepend = filename.substr ( fullPath.rfind( "/" ) + 1, std::string::npos );
  std::string			fullname = "";
  std::string			themeSubDir = "/current";
  std::string			subDirPrepend = "/" + filename.substr ( 0, fullPath.rfind( "/" ) );
  bool				hasSubDirPrepend = subDirPrepend.compare ( "/" );
  std::vector<std::string>	dirList;

  yuiMilestone () << "Preferring subdir: " << progSubDir << std::endl;

  if ( hasSubDirPrepend )	// prefer subdir prepended to filename
  {
    if ( isThemeDir )		// prefer /current inside THEMEDIR
    {
      dirList.push_back ( directory + "/" + progSubDir + themeSubDir + subDirPrepend );
      dirList.push_back ( directory + themeSubDir + subDirPrepend );
    }

    dirList.push_back ( directory + "/" + progSubDir + subDirPrepend );
    dirList.push_back ( directory + subDirPrepend );
  }

  if ( isThemeDir )		// prefer /current inside THEMEDIR
  {
    dirList.push_back ( directory + "/" + progSubDir + themeSubDir );
    dirList.push_back ( directory + themeSubDir );
  }

				// the "usual" lookup
  dirList.push_back ( directory + "/" + progSubDir );
  dirList.push_back ( directory );

  for ( std::vector<std::string>::const_iterator x = dirList.begin () ; x != dirList.end () && fullPath.compare ( "" ) == 0 ; ++x )
  {
    yuiMilestone () << "scanning subdir: " << *x << std::endl;

    std::vector<std::string> fileList = lsDir( *x );

    for ( std::vector<std::string>::const_iterator i = fileList.begin () ; i != fileList.end () && fullPath.compare ( "" ) == 0 ; ++i )
    {
      if ( *i != "." && *i != ".." )		// filter out parent and curdir
      {
	fullname =  directory + "/" + *i;
	if ( *i == filename )
	  fullPath = fullname;
	else
	{
	  fullPath = lookRecursive ( fullname, filenameNoPrepend );
	}
      }
    }
  }

  if( fullPath.compare ( "" ) != 0 )
    yuiMilestone() << "Found " << filename << " in " << dir() << std::endl;
  else
  {
    yuiMilestone() << "Could NOT find " << filename << " by looking recursive inside " << directory << std::endl;
    fullPath = filename;
  }
}

YPath::~YPath()
{
}

std::vector<std::string> YPath::lsDir( const std::string & directory )
{
  std::vector<std::string>	fileList;
  DIR *				dir;
  struct dirent *		ent;

  if ( ( dir = opendir( directory.c_str () ) ) != NULL )
  {
    yuiMilestone() << "Looking in " << directory << std::endl;

    while ( ( ent = readdir ( dir ) ) != NULL )
      fileList.push_back ( ent -> d_name );

    closedir ( dir );
  }
  else
    yuiMilestone() << "\"" << directory << "\" does NOT exist." << std::endl;

  return fileList;
}

std::string YPath::lookRecursive( const std::string & directory, const std::string & filename )
{
  std::vector<std::string>	fileList = lsDir( directory );
  std::string			file = "";
  std::string			fullname;

  for ( std::vector<std::string>::const_iterator i = fileList.begin() ; i != fileList.end() && file.compare ( "" ) == 0 ; ++i )
  {
    if ( *i != "." && *i != ".." )            // filter out parent and curdir
    {
      fullname =  directory + "/" + ( *i );
      if ( *i == filename )
	file = fullname;
      else
      {
	file = lookRecursive ( fullname, filename );
      }
    }
  }
  return file;
}

std::string YPath::path()
{
  return fullPath;
}

std::string YPath::dir()
{
  return fullPath.substr ( 0, fullPath.rfind( "/" ) );
}

/*
  Copyright (c) 2012 Björn Esser

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
  SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
  THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


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
#include "Libyui_config.h"

#define YUILogComponent "ui"
#include "YUILog.h"

YPath::YPath ( const std::string & directory, const std::string & filename )
{
  yuiMilestone () << "Given filename: " << filename << std::endl;

  bool				isThemeDir = ! directory.compare ( THEMEDIR );
  std::string			progSubDir = YSettings::progDir ();
  std::string			fullname = "";
  std::string			themeSubDir = "/current";
  size_t			splitPos = fullPath.rfind( "/" );
  bool				hasProgSubDir = progSubDir.compare ( "" );
  bool				hasSubDirPrepend = ( splitPos != std::string::npos );
  std::string			filenameNoPrepend = filename.substr ( splitPos + 1, std::string::npos );
  std::string			subDirPrepend = "";
  std::vector<std::string>	dirList;

  if ( hasSubDirPrepend )
    subDirPrepend = filename.substr ( 0, splitPos );

  yuiMilestone () << "Preferring subdir: " << progSubDir << std::endl;
  yuiMilestone () << "Subdir given with filename: " << subDirPrepend << std::endl;
  yuiMilestone () << "Looking for: " << filenameNoPrepend << std::endl;

  if ( hasSubDirPrepend )	// prefer subdir prepended to filename
  {
    if ( isThemeDir )		// prefer /current inside THEMEDIR
    {
      if ( hasProgSubDir )
        dirList.push_back ( directory + "/" + progSubDir + themeSubDir + "/" + subDirPrepend );

      dirList.push_back ( directory + themeSubDir + "/" + subDirPrepend );
    }
    if ( hasProgSubDir )
      dirList.push_back ( directory + "/" + progSubDir + "/" + subDirPrepend );

    dirList.push_back ( directory + "/" + subDirPrepend );
  }

  if ( isThemeDir )		// prefer /current inside THEMEDIR
  {
    if ( hasProgSubDir )
      dirList.push_back ( directory + "/" + progSubDir + themeSubDir );

    dirList.push_back ( directory + themeSubDir );
  }

				// the "usual" lookup
  if ( hasProgSubDir )
    dirList.push_back ( directory + "/" + progSubDir );

  dirList.push_back ( directory );

  for ( std::vector<std::string>::const_iterator x = dirList.begin () ; x != dirList.end () && fullPath.compare ( "" ) == 0 ; ++x )
  {
    std::vector<std::string> fileList = lsDir( *x );

    for ( std::vector<std::string>::const_iterator i = fileList.begin () ; i != fileList.end () && fullPath.compare ( "" ) == 0 ; ++i )
    {
      if ( *i != "." && *i != ".." )		// filter out parent and curdir
      {
	fullname =  directory + "/" + *i;
	if ( *i == filenameNoPrepend )
	  fullPath = fullname;
	else
	{
	  fullPath = lookRecursive ( fullname, filenameNoPrepend );
	}
      }
    }
  }

  if( fullPath.compare ( "" ) != 0 )
    yuiMilestone() << "Found " << filenameNoPrepend << " in " << dir() << std::endl;
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

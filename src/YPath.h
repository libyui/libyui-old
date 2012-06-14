/*
 |****************************************************************************
 |
 | Copyright (c) 2012 Björn Esser
 | All Rights Reserved.
 |
 | This file is part of libYUI which is
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


/*-/

  File:		YPath.h

  Author:	Björn Esser <bjoern.esser@gmail.com>

/-*/

#ifndef YPath_h
#define YPath_h

#include <string>
#include <vector>

/**
 * Finds files (e.g. plugins or theme pixmaps) recursively inside
 * a directory.
 **/
class YPath
{

public:

/**
 * Constructor
 *
 * to be called with the directory where to look inside
 * and filename which to lookup.
 *
 * YSettings::progSubDir will be preferred by the lookup.
 **/
  YPath( const std::string & directory, const std::string & filename );
/**
 * Destructor
 **/
  ~YPath();
/**
 * Returns the full path of the file if found;
 * if not found just the filename given in constructor.
 **/
  std::string path();
/**
 * Returns the directory where the file is found;
 * if not found just the subdir part (if there's any) of
 * the filename given in constructor.
 **/
  std::string dir();

private:

  std::vector<std::string> lsDir( const std::string & directory );
  std::string lookRecursive( const std::string & directory, const std::string & filename );
  std::string fullPath;

};

#endif // YUIDIRFINDER_H

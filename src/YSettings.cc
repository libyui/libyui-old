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

  File:		YSettings.cc

  Author:	Björn Esser <bjoern.esser@gmail.com>

/-*/

#include "YSettings.h"
#include "YUIException.h"

#define YUILogComponent "ui"
#include "YUILog.h"

using namespace std;

YSettings * YSettings::_instance = 0;
string YSettings::progSubDir = "";

YSettings * YSettings::access ()
{
  if ( ! _instance )
    _instance = new YSettings ();
  return _instance;
}

YSettings::YSettings()
{
  progSubDir = "";
}

YSettings::~YSettings ()
{
}

void YSettings::setProgSubDir( string directory )
{
  if ( progSubDir.compare ( "" ) == 0 )
  {
    progSubDir = directory;
    yuiMilestone () << "Set progSubDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "progSubDir is now locked." << endl;
  }
  else
  {
    yuiMilestone () << "Can't set progSubDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "It is locked to: \"" << progSubDir << "\"" << endl;
    YUI_THROW ( YUIException ( "progSubDir is locked to: \"" + progSubDir + "\"" ) );
  }
}

string YSettings::getProgSubDir ()
{
  return progSubDir;
}

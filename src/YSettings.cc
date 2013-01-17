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

  File:		YSettings.cc

  Author:	Björn Esser <bjoern.esser@gmail.com>

/-*/

#include "YSettings.h"
#include "YUIException.h"

#define YUILogComponent "ui"
#include "YUILog.h"
#include <yui/Libyui_config.h>

using std::endl;

YSettings  YSettings::_instance;

YSettings * YSettings::access ()
{
  return &_instance;
}

YSettings::YSettings() : progSubDir(), progIconDir(), progThemeDir()
{
}

YSettings::~YSettings ()
{
}

void YSettings::setProgSubDir( std::string directory )
{
  if ( progSubDir.empty() )
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

std::string YSettings::getProgSubDir ()
{
  yuiMilestone () << "progSubDir: \"" << progSubDir << "\"" << endl;

  return progSubDir;
}


void YSettings::setIconsDir( std::string directory )
{
  if ( progIconDir.empty() )
  {
    progIconDir = directory;
    yuiMilestone () << "Set progIconDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "progIconDir is now locked." << endl;
  }
  else
  {
    yuiMilestone () << "Can't set progIconDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "It is locked to: \"" << progIconDir << "\"" << endl;
    YUI_THROW ( YUIException ( "progIconDir is locked to: \"" + progIconDir + "\"" ) );
  }
}

std::string YSettings::getIconsDir ()
{
  if (progIconDir.size())
  {
    yuiMilestone () << "progIconDir: \"" << progIconDir << "\"" << endl;
    return progIconDir;
  }
  else if (progSubDir.size())
    return progSubDir + "/icons/";
  
  return THEMEDIR "/icons/";
}

void YSettings::setThemeDir( std::string directory )
{
  if ( progThemeDir.empty() )
  {
    progThemeDir = directory;
    yuiMilestone () << "Set progThemeDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "progThemeDir is now locked." << endl;
  }
  else
  {
    yuiMilestone () << "Can't set progThemeDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "It is locked to: \"" << progThemeDir << "\"" << endl;
    YUI_THROW ( YUIException ( "progThemeDir is locked to: \"" + progThemeDir + "\"" ) );
  }
}

std::string YSettings::getThemeDir ()
{
  if (progThemeDir.size())
  {
    yuiMilestone () << "progThemeDir: \"" << progThemeDir << "\"" << endl;
    return progThemeDir;
  }
  else if (progSubDir.size())
    return progSubDir + "/theme/";

  return THEMEDIR "/current/wizard/";
}
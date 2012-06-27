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

using std::endl;

YSettings * YSettings::_instance = 0;
std::string YSettings::progSubDir = "";

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

void YSettings::setProgSubDir( std::string directory )
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

std::string YSettings::getProgSubDir ()
{
  return progSubDir;
}

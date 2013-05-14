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

#include <algorithm>		// loadedPluginsCount
#include "YSettings.h"
#include "YUIException.h"
#include "YUILoader.h"		// YUIPlugin_NCurses

#define YUILogComponent "ui"
#include "YUILog.h"
#include "Libyui_config.h"

using std::endl;

std::string  YSettings::_progDir = "";
std::string  YSettings::_iconDir = "";
std::string  YSettings::_themeDir = "";
std::string  YSettings::_localeDir = "";
std::string  YSettings::_uiName = "";
std::string  YSettings::_loadedPlugins = "";

YSettings::YSettings() 
{
}

YSettings::~YSettings ()
{
}

void YSettings::setProgDir( std::string directory )
{
  if ( _progDir.empty() )
  {
    _progDir = directory;
    yuiMilestone () << "Set progDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "progDir is now locked." << endl;
  }
  else
  {
    yuiMilestone () << "Can't set progDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "It is locked to: \"" << _progDir << "\"" << endl;
    YUI_THROW ( YUIException ( "progSubDir is locked to: \"" + _progDir + "\"" ) );
  }
}

std::string YSettings::progDir ()
{
  yuiMilestone () << "progDir: \"" << _progDir << "\"" << endl;

  return _progDir;
}


void YSettings::setIconDir( std::string directory )
{
  if ( _iconDir.empty() )
  {
    _iconDir = directory;
    yuiMilestone () << "Set iconDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "iconDir is now locked." << endl;
  }
  else
  {
    yuiMilestone () << "Can't set iconDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "It is locked to: \"" << _iconDir << "\"" << endl;
    YUI_THROW ( YUIException ( "progIconDir is locked to: \"" + _iconDir + "\"" ) );
  }
}

std::string YSettings::iconDir ()
{
  if (_iconDir.size())
  {
    yuiMilestone () << "iconDir: \"" << _iconDir << "\"" << endl;
    return _iconDir;
  }
  else if (_progDir.size())
    return _progDir + "/icons/";
  
  return THEMEDIR "/icons/";
}

void YSettings::setThemeDir( std::string directory )
{
  if ( _themeDir.empty() )
  {
    _themeDir = directory;
    yuiMilestone () << "Set themeDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "themeDir is now locked." << endl;
  }
  else
  {
    yuiMilestone () << "Can't set themeDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "It is locked to: \"" << _themeDir << "\"" << endl;
    YUI_THROW ( YUIException ( "themeDir is locked to: \"" + _themeDir + "\"" ) );
  }
}

std::string YSettings::themeDir ()
{
  if ( _themeDir.size() )
  {
    yuiMilestone () << "themeDir: \"" << _themeDir << "\"" << endl;
    return _themeDir;
  }
  else if ( _progDir.size() )
  {
    //back compatibility if setProgSubDir is set to "/usr/share/YaST2"
    return _progDir + "/theme/current/wizard/";
  }

  return THEMEDIR "/current/wizard/";
}


void YSettings::setLocaleDir( std::string directory )
{
  if ( _localeDir.empty() )
  {
    _localeDir = directory;
    yuiMilestone () << "Set localeDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "localeDir is now locked." << endl;
  }
  else
  {
    yuiMilestone () << "Can't set localeDir to \"" << directory << "\"" << endl;
    yuiMilestone () << "It is locked to: \"" << _localeDir << "\"" << endl;
    YUI_THROW ( YUIException ( "localeDir is locked to: \"" + _localeDir + "\"" ) );
  }
}

std::string YSettings::localeDir ()
{
  if ( _localeDir.size() )
  {
    yuiMilestone () << "localeDir: \"" << _localeDir << "\"" << endl;
    return _localeDir;
  }
  else if ( _progDir.size() )
  {
    //back compatibility if ProgDir is set to "/usr/share/YaST2"
    return _progDir + "/locale/";
  }

  return "/usr/share/locale/";
}


void YSettings::setUiName( std::string name )
{
  if ( _uiName.empty() )
  {
    _uiName = name;
    yuiMilestone () << "Set uiName to \"" << name << "\"" << endl;
    ( void ) isGui(); // log output: whether we have a GUI or not.
    yuiMilestone () << "uiName is now locked." << endl;
    addLoadedPlugin( "ui" );
  }
  else
  {
    yuiMilestone () << "Can't set uiName to \"" << name << "\"" << endl;
    yuiMilestone () << "It is locked to: \"" << _uiName << "\"" << endl;
    YUI_THROW ( YUIException ( "uiName is locked to: \"" + _uiName + "\"" ) );
  }
}

std::string YSettings::uiName ()
{
  if ( _uiName.size() )
  {
    yuiMilestone () << "uiName is: \"" << _uiName << "\"" << endl;
    return _uiName;
  }

  YUI_THROW ( YUIPluginException ( "uiName is not set, which means no ui-plugin was loaded!" ) );
  return "";
}

bool YSettings::isGui ()
{
  bool guiAvail = !( uiName().compare( YUIPlugin_NCurses ) == 0 );
  yuiMilestone () << "We have a GUI is: \"" << ( guiAvail ? "TRUE" : "FALSE" ) << "\"" << endl;
  return guiAvail;
}


void YSettings::addLoadedPlugin ( std::string name )
{

  if ( name.size() != 0 )
  {
    yuiMilestone () << "Loaded plugins were: \"" << _loadedPlugins << "\"" << endl;
    ( void ) loadedPluginsCount(); // log the number of loaded plugins before adding.

    if ( _loadedPlugins.size() != 0 )
      _loadedPlugins.append( ";" );

    _loadedPlugins.append( name );
  }
  else
  {
    yuiMilestone () << "Will not add an empty string !!!" << endl;
  }

  yuiMilestone () << "Loaded plugins are: \"" << _loadedPlugins << "\"" << endl;
  ( void ) loadedPluginsCount(); // log the number of loaded plugins after adding.
}

std::string YSettings::loadedPlugins ()
{
  yuiMilestone () << "Loaded plugins are: \"" << _loadedPlugins << "\"" << endl;
  return _loadedPlugins;
}

size_t YSettings::loadedPluginsCount ()
{
  if ( _loadedPlugins.size() != 0 )
  {
    size_t pluginCount = std::count(_loadedPlugins.begin(), _loadedPlugins.end(), ';') + 1;
    yuiMilestone () << "The number of loaded plugins is: \"" << pluginCount << "\"" << endl;
    return pluginCount;
  }

  yuiMilestone () << "The number of loaded plugins is: \"0\"" << endl;
  return 0;
}


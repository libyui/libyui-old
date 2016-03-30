/*
  Copyright (C) 2013 Angelo Naselli <anaselli at linux dot it>

  This file is part of libyui project

  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) version 3.0 of the License. This library
  is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
  License for more details. You should have received a copy of the GNU
  Lesser General Public License along with this library; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301 USA
*/
#define YUILogComponent "ew"
#include "YUILog.h"

#include "YUI.h"
#include "YUILoader.h"
#include "YUIException.h"
#include "YExternalWidgets.h"
#include "YExternalWidgetFactory.h"

#include <map>
#include <string>

std::map<std::string, YExternalWidgets *> YExternalWidgets::_externalWidgets;

YExternalWidgets::YExternalWidgets(const std::string& name) : _name(name), _factory(0)
{
  if (!YUI::ui())
    YUI_THROW( YUIException( "UI must be initialized first" ) );

  yuiMilestone() << "Creating Libyui External Widgets object" <<  std::endl;

  std::pair<std::map<std::string, YExternalWidgets *>::iterator, bool> ret;
  ret = _externalWidgets.insert ( std::pair<std::string, YExternalWidgets *>(_name, this));
  if (ret.second==false) {
    std::string errorString = _name;
    errorString.append(" already created");
    YUI_THROW( YUIException( errorString ) );
  }
}

YExternalWidgets::~YExternalWidgets()
{
  delete _factory;

  _externalWidgets.erase(_name);
}

YExternalWidgets* YExternalWidgets::externalWidgets(const std::string& name)
{
  std::map<std::string, YExternalWidgets *>::iterator it;

  it = _externalWidgets.find(name);
  if (it == _externalWidgets.end())
  {
    YUILoader::loadExternalWidgets(name);
  }

  return _externalWidgets[name];
}

YExternalWidgetFactory* YExternalWidgets::externalWidgetFactory(const std::string& name)
{
  return YExternalWidgets::externalWidgets(name)->externalWidgetFactory();
}

YExternalWidgetFactory* YExternalWidgets::externalWidgetFactory()
{
  if (!YUI::ui())
    YUI_THROW( YUIException( "UI must be initialized first" ) );

  if ( !_factory )
        _factory = this->createExternalWidgetFactory();

  YUI_CHECK_PTR( _factory );

  return _factory;
}


/**
 * Helper class to make sure the EW is properly shut down.
 **/
class YExternalWidgetsTerminator
{
public:
    YExternalWidgetsTerminator() {}

    /**
     * Destructor.
     *
     * If there still is a EW, it will be deleted.
     * If there is none, this will do nothing.
     **/
    ~YExternalWidgetsTerminator();
};


YExternalWidgetsTerminator::~YExternalWidgetsTerminator()
{
  // Let's copy map to avoid content deletion when removing ExternalWidgets objects
  std::map <std::string, YExternalWidgets* > ew = YExternalWidgets::_externalWidgets;
  std::map<std::string, YExternalWidgets *>::iterator it;

  for (it= ew.begin(); it != ew.end(); it++)
  {
    yuiMilestone() << "Shutting down " << it->first << " External Widgets" << std::endl;
    delete it->second;
  }
}


/**
 * Static YExternalWidgetsTerminator instance: It will make sure the EW is deleted in its
 * global destructor. If the EW is already destroyed, it will do nothing. If
 * there still is a EW object, it will be deleted.
 *
 * This is particularly important for the NCurses EW so the terminal settings
 * are properly restored.
 **/
static YExternalWidgetsTerminator weTerminator;


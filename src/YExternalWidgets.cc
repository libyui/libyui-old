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
#define YUILogComponent "we"
#include "YUILog.h"

#include "YUI.h"
#include "YUIException.h"
#include "YExternalWidgets.h"

YExternalWidgets * YExternalWidgets::_externalWidgets = 0;


YExternalWidgets::YExternalWidgets()
{
  if (!YUI::ui())
    YUI_THROW( YUIException( "UI must be initialized first" ) );
  
  yuiMilestone() << "Creating Libyui Widget Extension object" <<  std::endl;
  
  _externalWidgets = this;
}

YExternalWidgets::~YExternalWidgets()
{
  _externalWidgets = 0;
}

YExternalWidgets* YExternalWidgets::externalWidgets()
{
  // we cannot ensure to have loaded it as for YUI, because the name 
  // of the plugin is user dependent
  return _externalWidgets;
}

YExternalWidgetsFactory* YExternalWidgets::externalWidgetsFactory()
{
  static YExternalWidgetsFactory * factory = 0;

  if (!YUI::ui())
    YUI_THROW( YUIException( "UI must be initialized first" ) );
  
  if (!_externalWidgets)
    YUI_THROW( YUIException( "WE (Widget Extension) must be initialized first" ) );
  
  if ( !factory )
        factory = externalWidgets()->createExternalWidgetsFactory();

  YUI_CHECK_PTR( factory );
  
  return factory;
}




/**
 * Helper class to make sure the WE is properly shut down.
 **/
class YExternalWidgetsTerminator
{
public:
    YExternalWidgetsTerminator() {}

    /**
     * Destructor.
     *
     * If there still is a WE, it will be deleted.
     * If there is none, this will do nothing.
     **/
    ~YExternalWidgetsTerminator();
};


YExternalWidgetsTerminator::~YExternalWidgetsTerminator()
{
    if ( YExternalWidgets::_externalWidgets )
    {
        yuiMilestone() << "Shutting down WE" << std::endl;
        delete YExternalWidgets::_externalWidgets;

        YExternalWidgets::_externalWidgets = 0;
    }
}


/**
 * Static YExternalWidgetsTerminator instance: It will make sure the WE is deleted in its
 * global destructor. If the WE is already destroyed, it will do nothing. If
 * there still is a WE object, it will be deleted.
 *
 * This is particularly important for the NCurses WE so the terminal settings
 * are properly restored.
 **/
static YExternalWidgetsTerminator weTerminator;

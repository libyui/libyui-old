/*
  |****************************************************************************
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

  File:		YGraphPlugin.h

  Author:	Arvin Schnell <aschnell@suse.de>

/-*/

#ifndef YGraphPlugin_h
#define YGraphPlugin_h

#include "YUIPlugin.h"

class YWidget;
class YGraph;


/**
 * Abstract base class for simplified access to UI plugins for graph widget.
 **/
class YGraphPlugin : public YUIPlugin
{
protected:
    /**
     * Constructor: Load the specified plugin library
     * from the standard UI plugin directory (/usr/lib/yui/).
     **/
    YGraphPlugin( const char * pluginLibBaseName )
	: YUIPlugin( pluginLibBaseName ) {}

    /**
     * Destructor. Calls dlclose() which will unload the plugin library if it
     * is no longer used, i.e. if the reference count dlopen() uses reaches 0.
     **/
    virtual ~YGraphPlugin() {}

public:
    /**
     * Create a graph widget.
     * Derived classes need to implement this.
     *
     * This might return 0 if the plugin lib could not be loaded or if the
     * appropriate symbol could not be located in the plugin lib.
     **/
    virtual YGraph * createGraph( YWidget * parent, const std::string & filename,
				  const std::string & layoutAlgorithm ) = 0;
};


#endif // YGraphPlugin_h

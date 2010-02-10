/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/


/*---------------------------------------------------------------------\
|                                                                      |  
|                      __   __    ____ _____ ____                      |  
|                      \ \ / /_ _/ ___|_   _|___ \                     |  
|                       \ V / _` \___ \ | |   __) |                    |  
|                        | | (_| |___) || |  / __/                     |  
|                        |_|\__,_|____/ |_| |_____|                    |  
|                                                                      |  
|                               core system                            | 
|                                                        (C) SuSE GmbH |  
\----------------------------------------------------------------------/ 

   File:	YUIComponentCreator.h

   Authors:	Stanislav Visnovsky <visnov@suse.de>
		Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YUIComponentCreator_h
#define YUIComponentCreator_h

#include <y2/Y2ComponentCreator.h>
#include <string>

class Y2Component;
using std::string;


/**
 * Component creator that provides access to the UI
 **/
class YUIComponentCreator : public Y2ComponentCreator
{
public:
    
    /**
     * Constructor.
     **/
    YUIComponentCreator();

    /**
     * This class creates server components
     **/
    virtual bool isServerCreator () const { return true; }

    /**
     * Creation function for Y2Components.
     *
     * This class can create Y2UIComponents named:
     *     - "UI"	(generic UI, for testsuites)
     *     - "ui"	(automatically choose a suitable UI)
     *     - "qt"	(Qt UI)
     *     - "ncurses"	(NCurses (text based) UI)
     *
     * In future versions also:
     *
     *     - "gtk"	(Gtk UI)
     *
     * For all other names, 0 is returned, so the Y2ComponentBroker will keep
     * on trying with other available creators. 
     **/
    virtual Y2Component * create( const char * name ) const;

    /**
     * Creation function for non-built-in components.
     **/
    virtual Y2Component * createInLevel( const char * name, int level, int currentLevel ) const;
    
    /**
     * Name space provider function.
     * This class can provide the "UI" name space.
     *
     * For all other names, 0 is returned, so the Y2ComponentBroker will keep
     * on trying with other available creators. 
     **/
    virtual  Y2Component * provideNamespace( const char * name );

protected:

    /**
     * Internal create method.
     **/
    Y2Component * createInternal( const string & name, bool isNamespace ) const;
};


#endif // YUIComponentCreator_h

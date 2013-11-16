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

#ifndef YExternalWidgets_h
#define YExternalWidgets_h

class YExternalWidgetsFactory;

/**
 * Abstract base class of a libYUI Widget Extension interface.
 **/
class YExternalWidgets
{
    friend class YExternalWidgetsTerminator;

protected:
    /**
     * Constructor.
     **/
    YExternalWidgets( );

public:

    /**
     * Destructor.
     **/
    virtual ~YExternalWidgets();


    /**
     * Access the global YUI external widgets.
     * 
     * NOTE that only one external widget plugin can be loaded, further implementation
     *      may allow more.
     **/
    static YExternalWidgets * externalWidgets();

    /**
     * Return the external widget factory that provides all the createXY() methods for
     * user defined widgets.
     *
     * This will create the factory upon the first call and return a pointer to
     * the one and only (singleton) factory upon each subsequent call.
     * This may throw exceptions if the factory cannot be created.
     * 
     * It is up to user extend YExternalWidgetsFactory to add createXY() methods in 
     * his/her implementation. So once YExternalWidgetsFactory is extended with 
     * all the createXY() methods, three sub-plugins must be defined one for each
     * supported graphical environment, e.g. Gtk, ncurses and QT, following the
     * libyui implementation rules.
     * 
     * For instance an external widgets plugin called yui-foo that needs Gtk, ncurses 
     * and QT specialization will require also yui-foo-gtk, yui-foo-ncurses and
     * yui-foo-qt plugin implementation.
     * 
     **/
    static YExternalWidgetsFactory * externalWidgetsFactory();

protected:

    /**
     * Create the external widgets factory that provides all the createXY() methods for
     *
     * Derived classes are required to implement this. Usually createXY() is virtual,
     * real implementation is demanded to derived classes that implement Gtk, ncurses and QT
     * specialization.
     **/
    virtual YExternalWidgetsFactory * createExternalWidgetsFactory() = 0;

private:

    static YExternalWidgets *_externalWidgets;
};

#endif // YExternalWidgets_h
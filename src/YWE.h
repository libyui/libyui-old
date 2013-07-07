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

#ifndef YWE_h
#define YWE_h

class YWidgetExtensionFactory;

/**
 * Abstract base class of a libYUI Widget Extension interface.
 **/
class YWE
{
    friend class YWETerminator;

protected:
    /**
     * Constructor.
     **/
    YWE( );

public:

    /**
     * Destructor.
     **/
    virtual ~YWE();


    /**
     * Access the global YUI Widget Extension.
     **/
    static YWE * we();

    /**
     * Return the widget extension factory that provides all the createXY() methods for
     * user defined extension widgets.
     *
     * This will create the factory upon the first call and return a pointer to
     * the one and only (singleton) factory upon each subsequent call.
     * This may throw exceptions if the factory cannot be created.
     * 
     * It is up to user extend YWidgetExtensionFactory to add createXY() methods.
     * 
     **/
    static YWidgetExtensionFactory * widgetExtensionFactory();

protected:

    /**
     * Create the widget extension factory that provides all the createXY() methods for
     *
     * Derived classes are required to implement this.
     **/
    virtual YWidgetExtensionFactory * createWidgetExtensionFactory() = 0;

private:

    static YWE *_we;
};

#endif // YWE_h
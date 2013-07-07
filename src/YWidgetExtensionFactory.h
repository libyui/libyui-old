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

#ifndef YWidgetExtensionFactory_h
#define YWidgetExtensionFactory_h

/**
 * Abstract widget factory for mandatory widgets.
 * Use YOptionalWidgetFactory for optional ("special") widgets.
 *
 * Refer to the respective widget's documentation (in the header file) for
 * documentation about the function parameters.
 **/
class YWidgetExtensionFactory
{
public:



protected:

    friend class YUI;
    friend class YWE;
    
    /**
     * Constructor.
     *
     * Use YWE::widgetExtensionFactory() to get the singleton for this class.
     **/
    YWidgetExtensionFactory() {}

    /**
     * Destructor.
     **/
    virtual ~YWidgetExtensionFactory() {}

}; // class YWidgetExtensionFactory

#endif // YWidgetExtensionFactory_h
/****************************************************************************

Copyright (c) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, contact Novell, Inc.

To contact Novell about this file by physical or electronic mail,
you may find current contact information at www.novell.com

****************************************************************************


  File:		YCPValueWidgetID.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPValueWidgetID_h
#define YCPValueWidgetID_h

#include <yui/YWidgetID.h>
#include <ycp/YCPValue.h>

using std::string;


/**
 * Widget ID class based on YCPValue.
 **/
class YCPValueWidgetID: public YWidgetID
{
public:

    /**
     * Constructor.
     **/
    YCPValueWidgetID( const YCPValue & value );

    /**
     * Check if this ID is equal to another.
     *
     * Reimplemented from YWidgetID.
     **/
    virtual bool isEqual( YWidgetID * otherID ) const;

    /**
     * Convert the ID value to string.
     * Used for logging and debugging.
     *
     * Reimplemented from YWidgetID.
     **/
    virtual string toString() const;

    /**
     * Return the ID value.
     **/
    YCPValue value() const;

private:
    
    YCPValue _value;
};



#endif // YCPValueWidgetID_h

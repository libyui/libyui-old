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

  File:		YFrame.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YFrame_h
#define YFrame_h

#include <string>
#include "YSingleChildContainerWidget.h"
#include "ImplPtr.h"

class YFramePrivate;


class YFrame : public YSingleChildContainerWidget
{
protected:
    /**
     * Constructor.
     **/
    YFrame( YWidget * parent, const std::string & label );

public:
    /**
     * Destructor.
     **/
    virtual ~YFrame();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YFrame"; }

    /**
     * Change the frame label.
     *
     * Derived classes should overwrite this, but call this base class function
     * in the overwritten function.
     **/
    virtual void setLabel( const std::string & newLabel );

    /**
     * Get the current frame label.
     **/
    std::string label() const;

    /**
     * Set a property.
     * Reimplemented from YWidget.
     *
     * This method may throw exceptions, for example
     *   - if there is no property with that name
     *   - if the expected type and the type mismatch
     *   - if the value is out of range
     *
     * This function returns 'true' if the value was successfully set and
     * 'false' if that value requires special handling (not in error cases:
     * those are covered by exceptions).
     **/
    virtual bool setProperty( const std::string & propertyName,
			      const YPropertyValue & val );

    /**
     * Get a property.
     * Reimplemented from YWidget.
     *
     * This method may throw exceptions, for example
     *   - if there is no property with that name
     **/
    virtual YPropertyValue getProperty( const std::string & propertyName );

    /**
     * Return this class's property set.
     * This also initializes the property set upon the first call.
     *
     * Reimplemented from YWidget.
     **/
    virtual const YPropertySet & propertySet();


private:

    ImplPtr<YFramePrivate> priv;
};


#endif // YFrame_h

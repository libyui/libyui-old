/*
  Copyright (c) [2019] SUSE LLC
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


/*-/

  File:		YItemSelector.h

  Author:	Stefan Hundhammer <shundhammer@suse.de>

/-*/

#ifndef YItemSelector_h
#define YItemSelector_h

#include "YSelectionWidget.h"
#include "YDescribedItem.h"


class YItemSelectorPrivate;

/**
 * Scrollable item selector widget with not only a label for each item, but
 * also a (possible multi-line) description and an optional icon.
 *
 * This widget supports both 1-of-n or n-of-m selection, i.e. it can act as a
 * (more verbose and more screen space consuming) replacement for YSelectionBox
 * or YMultiSelectionBox.
 **/
class YItemSelector: public YSelectionWidget
{
public:

    /**
     * Constructor.
     **/
    YItemSelector( YWidget *    parent,
                   bool         enforceSingleSelection = true );

    /**
     * Destructor.
     **/
    virtual ~YItemSelector();

    /**
     * Return a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const;

    /**
     * Return the number of visible items (i.e. items that are visible without
     * scrolling). This is used to calculate the preferred height. If the
     * widget gets more or less screen space than desired due to layout
     * constraints, this number is not updated; this is purely the desired
     * value for initializing layout negotiations.
     **/
    int visibleItems() const;

    /**
     * Set the number of visible items. When changing this, make sure to
     * recalculate the layout (YDialog::recalc()) so the change has any effect.
     *
     * Derived classes are free to reimplement this, but they should call this
     * base class method in the overloaded function.
     **/
    virtual void setVisibleItems( int newVal );

    /**
     * Set a property.
     * Reimplemented from YWidget.
     *
     * This function may throw YUIPropertyExceptions.
     *
     * This function returns 'true' if the value was successfully set and
     * 'false' if that value requires special handling (not in error cases:
     * those are covered by exceptions).
     **/
    virtual bool setProperty( const std::string &    propertyName,
			      const YPropertyValue & val );

    /**
     * Get a property.
     * Reimplemented from YWidget.
     *
     * This method may throw YUIPropertyExceptions.
     **/
    virtual YPropertyValue getProperty( const std::string & propertyName );

    /**
     * Return this class's property set.
     * This also initializes the property upon the first call.
     *
     * Reimplemented from YWidget.
     **/
    virtual const YPropertySet & propertySet();

    /**
     * The name of the widget property that will return user input.
     * Inherited from YWidget.
     **/
    const char * userInputProperty() { return YUIProperty_Value; }


private:

    ImplPtr<YItemSelectorPrivate> priv;

};      // class YItemSelector

#endif  // YItemSelector_h

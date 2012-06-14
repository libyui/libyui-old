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

  File:		YDumbTab.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YDumbTab_h
#define YDumbTab_h

#include "YSelectionWidget.h"

class YDumbTabPrivate;

/**
 * DumbTab: A very simple tab widget that can display and switch between a
 * number of tabs, but will only deliver the "user clicked on tab " event very
 * much like a PushButton does. Actually exchanging the content of the tab is
 * left to the application.
 *
 * DumbTab accepts a single child widget.
 **/
class YDumbTab : public YSelectionWidget
{
protected:
    /**
     * Constructor.
     **/
    YDumbTab( YWidget * parent );

public:
    /**
     * Destructor.
     **/
    virtual ~YDumbTab();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YDumbTab"; }

    /**
     * Add an item (a tab page).
     *
     * Reimplemented from YSelectionWidget.
     *
     * Derived classes can overwrite this function, but they should call this
     * base class function in the new implementation.
     **/
    virtual void addItem( YItem * item );

    /**
     * Notification that any shortcut of any item was changed by the shortcut
     * conflict manager.
     *
     * Derived classes should reimplement this.
     **/
    virtual void shortcutChanged() {}

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

    /**
     * Get the string of this widget that holds the keyboard shortcut.
     * Notice that since YDumbTab has one shortcut for each tab page (for each
     * item), this is not meaningful for this widget class.
     *
     * Check YItemShortcut in YShortcut.{cc,h} for more details.
     *
     * Reimplemented from YSelectionWidget.
     **/
    virtual std::string shortcutString() const { return ""; }

    /**
     * Set the string of this widget that holds the keyboard shortcut.
     * Since YDumbTab doesn't have a shortcut for the widget itself (only for
     * the tab pages, i.e. the items), this will simply trigger a
     * shortcutChanged() notification.
     *
     * Reimplemented from YSelectionWidget.
     **/
    virtual void setShortcutString( const std::string & str ) { shortcutChanged(); }

    /**
     * Returns 'true' if this widget is stretchable in the specified dimension.
     * In this case, the stretchability of the single child is returned.
     *
     * Reimplemented from YWidget.
     **/
    virtual bool stretchable( YUIDimension dim ) const;

    /**
     * Descriptive label for debugging. Derived from this widget's only child
     * (if there is one).
     **/
    virtual std::string debugLabel() const;

private:

    // Disable unwanted base class methods
    std::string label() const;
    virtual void setLabel( const std::string & newLabel ) {};


    ImplPtr<YDumbTabPrivate> priv;
};


#endif // YDumbTab_h

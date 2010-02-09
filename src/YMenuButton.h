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
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:		YMenuButton.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YMenuButton_h
#define YMenuButton_h

#include "YSelectionWidget.h"
#include "YMenuItem.h"

class YMenuItem;
class YMenuButtonPrivate;


/**
 * MenuButton: Similar to PushButton, but with several actions: Upon clicking
 * on a MenuButton (or activating it with the keyboard), a pop-up menu opens
 * where the user can activate an action. Menu items in that pop-up menu can
 * have submenus (that will pop up in separate pop-up menus).
 *
 * Internally, this widget is more similar to the Tree widget. The difference
 * is that it does not keep a "selected" status, but triggers an action right
 * away, just like a PushButton. Like PushButton, MenuButton sends an event
 * right away when the user selects an item (clicks on a menu item or activates
 * it with the keyboard). Items that have a submenu never send an event, they
 * simply open their submenu when activated.
 **/
class YMenuButton : public YSelectionWidget
{
protected:
    /**
     * Constructor.
     *
     * 'label' is the user-visible text on the button (not above it like all
     * other SelectionWidgets).
     **/
    YMenuButton( YWidget * parent, const string & label );

public:
    /**
     * Destructor.
     **/
    virtual ~YMenuButton();
     
    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YMenuButton"; }

    /**
     * Rebuild the displayed menu tree from the internally stored YMenuItems.
     *
     * The application should call this (once) after all items have been added
     * with addItem(). YMenuButton::addItems() calls this automatically.
     *
     * Derived classes are required to implement this.
     **/
    virtual void rebuildMenuTree() = 0;

    /**
     * Add multiple items. For some UIs, this can be more efficient than
     * calling addItem() multiple times. This function also automatically calls
     * resolveShortcutConflicts() and rebuildMenuTree() at the end.
     *
     * Derived classes can overwrite this function, but they should call this
     * base class function at the end of the new implementation.
     *
     * Reimplemented from YSelectionWidget.
     **/
    virtual void addItems( const YItemCollection & itemCollection );

    /**
     * Add one item. This widget assumes ownership of the item object and will
     * delete it in its destructor.
     *
     * This reimplementation will an index to the item that is unique for all
     * items in this MenuButton. That index can be used later with
     * findMenuItem() to find the item by that index.
     *
     * Reimplemented from YSelectionWidget.
     **/
    virtual void addItem( YItem * item_disown );

    /**
     * Delete all items.
     *
     * Reimplemented from YSelectionWidget.
     **/
    virtual void deleteAllItems();

    /**
     * Resolve keyboard shortcut conflicts: Change shortcuts of menu items if
     * there are duplicates in the respective menu level.
     *
     * This has to be called after all items are added, but before rebuildMenuTree()
     * (see above). YMenuButton::addItems() calls this automatically.
     **/
    void resolveShortcutConflicts();

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
    virtual bool setProperty( const string & propertyName,
			      const YPropertyValue & val );

    /**
     * Get a property.
     * Reimplemented from YWidget.
     *
     * This method may throw YUIPropertyExceptions.
     **/
    virtual YPropertyValue getProperty( const string & propertyName );

    /**
     * Return this class's property set.
     * This also initializes the property upon the first call.
     *
     * Reimplemented from YWidget.
     **/
    virtual const YPropertySet & propertySet();

protected:

    /**
     * Recursively find the first menu item with the specified index.
     * Returns 0 if there is no such item.
     **/
    YMenuItem * findMenuItem( int index );
    
    /**
     * Recursively find the first menu item with the specified index
     * from iterator 'begin' to iterator 'end'.
     *
     * Returns 0 if there is no such item.
     **/
    YMenuItem * findMenuItem( int index, YItemConstIterator begin, YItemConstIterator end );

    /**
     * Alias for findMenuItem(). Reimplemented to ensure consistent behaviour
     * with YSelectionWidget::itemAt(). 
     **/
    YMenuItem * itemAt( int index )
	{ return findMenuItem( index ); }

private:

    /**
     * Assign a unique index to all items from iterator 'begin' to iterator 'end'.
     **/
    void assignUniqueIndex( YItemIterator begin, YItemIterator end );


    ImplPtr<YMenuButtonPrivate> priv;
};


#endif // YMenuButton_h

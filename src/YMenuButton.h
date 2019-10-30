/*
  Copyright (C) 2000-2012 Novell, Inc
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
    YMenuButton( YWidget * parent, const std::string & label );

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
     * @note please do not forget to call after adding all elements
     * #resolveShortcutConflicts and #rebuildMenuTree in this order. It is
     * important to call it after all submenus are added otherwise it won't
     * have proper shortcuts and won't be rendered.
     * @see examples/MenuButton.cc.
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
    virtual bool setProperty( const std::string & propertyName,
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
     * Return item in the tree which matches path of labels or nullptr in case no
     * item with such label was found and is a leaf, as other nodes do not trigger
     * actions except showing children items.
     * Accepts vector of strings which denote path to the node.
     **/
    YMenuItem * findItem( std::vector<std::string> & path ) const;

    /**
    * Activate the item selected in the tree. Can be used in tests to simulate user input.
    *
    * Derived classes are required to implement this.
    **/
    virtual void activateItem( YMenuItem * item ) = 0;

    /**
     * Recursively find the first menu item with the specified index.
     * Returns 0 if there is no such item.
     **/
    YMenuItem * findMenuItem( int index );

protected:

    /**
     * Recursively find the first menu item with the specified index
     * from iterator 'begin' to iterator 'end'.
     *
     * Returns 0 if there is no such item.
     **/
    YMenuItem * findMenuItem( int index, YItemConstIterator begin, YItemConstIterator end );

    /**
     * Recursively looks for the first item in the tree of the menu items
     * using depth first search.
     * Return nullptr if item which matches full path is not found.
     * Path is a vector of strings, where next element is a child item, so
     * in case one needs to select File->Export->As PDF, for instance,
     * Vector will look like [ "File", "Export", "As PDF" ].
     */
    YMenuItem * findItem( std::vector<std::string>::iterator path_begin,
                          std::vector<std::string>::iterator path_end,
                          YItemConstIterator begin,
                          YItemConstIterator end ) const;

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

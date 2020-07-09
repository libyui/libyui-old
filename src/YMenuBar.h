/*
  Copyright (c) [2020] SUSE LLC
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

  File:		YMenuBar.h

  Author:	Stefan Hundhammer <shundhammer@suse.de>

/-*/

#ifndef YMenuBar_h
#define YMenuBar_h

#include "YSelectionWidget.h"
#include "YMenuItem.h"

class YMenuBarPrivate;


/**
 * A classical menu bar for pulldown menus.
 *
 * Use this only when appropriate: In most places, YaST follows a wizard-driven
 * UI strategy, asking the user (ideally) one question at a time, with a [Next]
 * and a [Back] button to move between wizard steps. One of the last steps is
 * usually presenting the collected information to the user until everything is
 * applied.
 *
 * A menu bar OTOH is meant for the opposite UI strategy: Presenting some kind
 * of document (which may also be a number of input fields) to the user as the
 * central point and providing lots of different operations on that document.
 * The two concepts don't mix very well, so use this widget with caution.
 *
 * A menu bar should only contain menus, no direct actions.
 **/
class YMenuBar: public YSelectionWidget
{
protected:
    /**
     * Constructor.
     **/
    YMenuBar( YWidget * parent );

public:
    /**
     * Destructor.
     **/
    virtual ~YMenuBar();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YMenuBar"; }

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
    virtual bool setProperty( const std::string    & propertyName,
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
     * Support for the Rest API for UI testing:
     *
     * Return the item in the tree which matches a path of labels. This
     * returns 0 if there is no such item or if it is not a leaf menu item.
     *
     * 'path' specifies the user-visible labels (i.e. the translated texts) of
     * each menu level ( ["File", "Export", "As XML"] ).
     **/
    YMenuItem * findItem( std::vector<std::string> & path ) const;

    /**
     * Support for the Rest API for UI testing:
     *
     * Activate the item selected in the tree.
     * This can be used in tests to simulate user input.
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
     * Check if all toplevel items are really menus, i.e. YMenuItems that have
     * children. This may throw a YUIException.
     **/
    void sanityCheck();

    /**
     * Recursively find the first menu item with the specified index
     * from iterator 'begin' to iterator 'end'.
     *
     * Returns 0 if there is no such item.
     **/
    YMenuItem * findMenuItem( int                index,
                              YItemConstIterator begin,
                              YItemConstIterator end );

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
                          YItemConstIterator                 begin,
                          YItemConstIterator                 end ) const;

    /**
     * Alias for findMenuItem(). Reimplemented to ensure consistent behaviour
     * with YSelectionWidget::itemAt().
     **/
    YMenuItem * itemAt( int index )
	{ return findMenuItem( index ); }

    /**
     * Assign a unique index to all items from iterator 'begin' to iterator 'end'.
     **/
    void assignUniqueIndex( YItemIterator begin, YItemIterator end );

private:

    ImplPtr<YMenuBarPrivate> priv;
};


#endif  // YMenuBar_h

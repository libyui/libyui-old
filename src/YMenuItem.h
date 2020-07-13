/*
  Copyright (C) 2000-2018 Novell, Inc
  Copyright (c) [2019-2020] SUSE LLC
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

  File:		YMenuItem.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YMenuItem_h
#define YMenuItem_h

#include "YTreeItem.h"



/**
 * Item class for menu items.
 *
 * This is used for plain menu items as well as for menus as well as for menu
 * separators:
 *
 * A menu is a menu item with child items.
 * A plain menu item does not have child items.
 * A menu separator has an empty label.
 **/
class YMenuItem: public YTreeItem
{
public:
    /**
     * Constructor for toplevel items.
     **/
    YMenuItem( const std::string & 	label,
	       const std::string & 	iconName = "" )
	: YTreeItem( label, iconName )
	{}

    /**
     * Constructors for items that have a parent item.
     *
     * They will automatically register this item with the parent item. The
     * parent assumes ownership of this item and will delete it in its (the
     * parent's) destructor.
     *
     * Consider using addItem() or addMenu() instead.
     **/
    YMenuItem( YMenuItem *		parent,
	       const std::string & 	label,
	       const std::string & 	iconName = "" )
	: YTreeItem( parent, label, iconName )
	{}


    /**
     * Destructor.
     *
     * This will delete all children.
     **/
    virtual ~YMenuItem() {}

    /**
     * Create a new menu item as a child of the current instance and return it.
     * The newly created object is owned by this instance.
     * This is meant for plain menu items, not for submenus.
     **/
    YMenuItem * addItem( const std::string & label,
                         const std::string & iconName = "" );

    /**
     * Create a new submenu as a child of the current instance and return it.
     * The newly created object is owned by this instance.
     * This is meant to be used for menu items that have children.
     **/
    YMenuItem * addMenu( const std::string & label,
                         const std::string & iconName = "" );

    /**
     * Create a menu separator as a child of the current instance and return it.
     * The newly created object is owned by this instance.
     **/
    YMenuItem * addSeparator();

    /**
     * Returns this item's parent item or 0 if it is a toplevel item.
     **/
    YMenuItem * parent() const
	{ return dynamic_cast<YMenuItem *> ( YTreeItem::parent() ); }

    /**
     * Return 'true' if this is a menu (or submenu), i.e. if it has any child
     * items.
     **/
    bool isMenu() const { return hasChildren(); }

    /**
     * Return 'true' if this is a menu separator, i.e. if it has an empty label.
     **/
    bool isSeparator() const { return label().empty(); }

    /**
     * Create a menu separator item and return it. The new separator is owned
     * by 'parent'.
     **/
    static YMenuItem * createSeparator( YMenuItem * parent)
        { return new YMenuItem( parent, "" ); }


private:

    // Disable unwanted base class methods

    bool isOpen() const  { return false; }
    void setOpen( bool ) {}
};


#endif // YMenuItem_h

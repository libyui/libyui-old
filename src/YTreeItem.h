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

  File:		YTreeItem.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YTreeItem_h
#define YTreeItem_h

#include "YItem.h"

using std::string;
using std::vector;


/**
 * Item class for tree items.
 *
 * This class implements children management.
 **/
class YTreeItem: public YItem
{
public:
    /**
     * Constructors for toplevel items.
     **/
    YTreeItem( const string &	label,
	       bool 		isOpen = false );

    YTreeItem( const string & 	label,
	       const string & 	iconName,
	       bool 		isOpen = false );

    /**
     * Constructors for items that have a parent item.
     *
     * They will automatically register this item with the parent item. The
     * parent assumes ownership of this item and will delete it in its (the
     * parent's) destructor.
     **/
    YTreeItem( YTreeItem * 	parent,
	       const string & 	label,
	       bool 		isOpen = false );

    YTreeItem( YTreeItem * 	parent,
	       const string & 	label,
	       const string & 	iconName,
	       bool 		isOpen = false );

    /**
     * Destructor.
     *
     * This will delete all children.
     **/
    virtual ~YTreeItem();

    /**
     * Return 'true' if this item has any child items.
     *
     * Reimplemented from YItem.
     **/
    virtual bool hasChildren() const { return ! _children.empty(); }

    /**
     * Return an iterator that points to the first child item of this item.
     *
     * Reimplemented from YItem.
     **/
    virtual YItemIterator	childrenBegin()		{ return _children.begin(); }
    virtual YItemConstIterator	childrenBegin() const	{ return _children.begin(); }

    /**
     * Return an iterator that points after the last child item of this item.
     *
     * Reimplemented from YItem.
     **/
    virtual YItemIterator 	childrenEnd() 		{ return _children.end(); }
    virtual YItemConstIterator	childrenEnd() const	{ return _children.end(); }

    /**
     * Add a child item to this item.
     *
     * Note that the constructors that accept a parent pointer will
     * automatically add themselves to their parent, so applications will
     * normally not have to call this function.
     **/
    virtual void addChild( YItem * item_disown );

    /**
     * Delete all child items.
     **/
    virtual void deleteChildren();

   /**
     * Return 'true' if this tree item should be displayed open (with its
     * children visible) by default.
     *
     * Notice that this will always return 'false' for tree items without
     * children.  
     **/
    bool isOpen() const;

    /**
     * Change the 'isOpen' flag.
     **/
    void setOpen( bool open );

    /**
     * Returns this item's parent item or 0 if it is a toplevel item.
     *
     * Reimplemented from YItem.
     **/
    virtual YTreeItem * parent() const { return _parent; }

private:

    YTreeItem *		_parent;
    YItemCollection	_children;
    bool 		_isOpen;
};


#endif // YTreeItem_h

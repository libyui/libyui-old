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

  File:		YItem.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YItem_h
#define YItem_h

#include <string>
#include <vector>

using std::string;
using std::vector;

class YItem;

typedef vector<YItem *> 			YItemCollection;
typedef YItemCollection::iterator		YItemIterator;
typedef YItemCollection::const_iterator		YItemConstIterator;


/**
 * Simple item class for SelectionBox, ComboBox, MultiSelectionBox etc. items.
 * This class provides stubs for children management.
 **/
class YItem
{
public:
    /**
     * Constructor with just the label and optionally the selected state.
     **/
    YItem( const string & label, bool selected = false )
	: _label( label )
	, _selected( selected )
	, _index( -1 )
	, _data( 0 )
	{}

    /**
     * Constructor with label and icon name and optionally the selected state.
     **/
    YItem( const string & label, const string & iconName, bool selected = false )
	: _label( label )
	, _iconName( iconName )
	, _selected( selected )
	, _index( -1 )
	, _data( 0 )
	{}

    /**
     * Destructor.
     **/
    virtual ~YItem() {}

    /**
     * Return this item's label. This is what the user sees in a dialog, so
     * this will usually be a translated text.
     **/
    string label() const { return _label; }

    /**
     * Set this item's label.
     **/
    void setLabel( const string & newLabel ) { _label = newLabel; }

    /**
     * Return this item's icon name.
     **/
    string iconName() const { return _iconName; }

    /**
     * Return 'true' if this item has an icon name.
     **/
    bool hasIconName() const { return ! _iconName.empty(); }

    /**
     * Set this item's icon name.
     **/
    void setIconName( const string & newIconName ) { _iconName = newIconName; }

    /**
     * Return 'true' if this item is currently selected.
     **/
    bool selected() const { return _selected; }

    /**
     * Select or unselect this item. This does not have any effect on any other
     * item; if it is desired that only one item is selected at any time, the
     * caller has to take care of that.
     **/
    void setSelected( bool sel = true ) { _selected = sel; }

    /**
     * Set this item's index.
     **/
    void setIndex( int index ) { _index = index; }

    /**
     * Return the index of this item (as set with setIndex() ).
     **/
    int index() const { return _index; }

    /**
     * Set the opaque data pointer for application use.
     *
     * Applications can use this to store the pointer to a counterpart of this
     * tree item. It is the application's responsibility to watch for dangling
     * pointers and possibliy deleting the data. All this class ever does with
     * this pointer is to store it.
     **/
    void setData( void * newData ) { _data = newData; }

    /**
     * Return the opaque data pointer.
     **/
    void * data() const { return _data; }

    //
    // Children management stubs.
    //
    // Derived classes that can handle child items should reimplement those
    // functions.  
    // The following default implementations don't do anything with children;
    // they act as if this item didn't have any children.
    //

    /**
     * Return 'true' if this item has any child items.
     **/
    virtual bool hasChildren() const { return false; }

    /**
     * Return an iterator that points to the first child item of this item.
     *
     * This default implementation returns the 'end' iterator of the
     * class-static always empty _noChildren YItemCollection.
     * It is safe to use this iterator in classic iterator loops:
     *
     *   for ( YItemIterator it = myItem->childrenBegin();
     *         it != myItem->childrenEnd();
     *         ++it )
     *	 {
     *       ...
     *   }
     *
     * The loop body will only ever be executed if this item is a derived class
     * that actually manages child items.
     **/
    virtual YItemIterator	childrenBegin()		{ return _noChildren.end(); }
    virtual YItemConstIterator	childrenBegin() const	{ return _noChildren.end(); }

    /**
     * Return an iterator that points after the last child item of this item.
     *
     * This default implementation returns the 'end' iterator of the
     * class-static always empty _noChildren YItemCollection.
     **/
    virtual YItemIterator	childrenEnd() 		{ return _noChildren.end(); }
    virtual YItemConstIterator	childrenEnd() const	{ return _noChildren.end(); }

    /**
     * Returns this item's parent item or 0 if it is a toplevel item.
     * This default implementation always returns 0.
     * Derived classes that handle children should reimplement this.
     **/
    virtual YItem * parent() const { return 0; }


private:

    string	_label;
    string	_iconName;
    bool	_selected;
    int		_index;
    void *	_data;

    /**
     * Static children collection that is always empty so the children
     * iterators of this base class have something valid to return.
     **/
    static YItemCollection _noChildren;
};



#endif // YItem_h

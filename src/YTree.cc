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

  File:		YTree.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YSelectionWidget.h"
#include "YTree.h"
#include "YTreeItem.h"

struct YTreePrivate
{
    YTreePrivate()
	: immediateMode( false )
	{}

    bool immediateMode;
};


YTree::YTree( YWidget * parent, const string & label, bool multiSelection, bool recursiveSelection )
    : YSelectionWidget( parent, label,
			! multiSelection,
			recursiveSelection ) 
    , priv( new YTreePrivate() )
{
    YUI_CHECK_NEW( priv );

    setDefaultStretchable( YD_HORIZ, true );
    setDefaultStretchable( YD_VERT,  true );
}


YTree::~YTree()
{
    // NOP
}


bool
YTree::immediateMode() const
{
    return priv->immediateMode;
}


void
YTree::setImmediateMode( bool immediateMode )
{
    priv->immediateMode = immediateMode;

    if ( immediateMode )
	setNotify( true );
}


void
YTree::addItems( const YItemCollection & itemCollection )
{
    YSelectionWidget::addItems( itemCollection );
    rebuildTree();
}


const YPropertySet &
YTree::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property itemID  		Value		The currently selected item
	 * @property itemID  		CurrentItem	The currently selected item
	 * @property list<itemID>  	CurrentBranch	List of IDs of current branch from root to current item
	 * @property itemList		Items		All items
	 * @property map<ItemID>	OpenItems 	Map of IDs of all open items - can only be queried, not set
	 * @property string  		Label		Caption above the tree
	 * @property string  		IconPath	Base path for icons
         * @property bool               MultiSelection  Flag: User can select multiple items (read-only)
	 */
	propSet.add( YProperty( YUIProperty_Value,		YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_CurrentItem,	YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_CurrentBranch,	YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_Items,		YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_OpenItems,		YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_Label,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_IconPath,		YStringProperty	 ) );
        propSet.add( YProperty( YUIProperty_SelectedItems,      YOtherProperty   ) );
        propSet.add( YProperty( YUIProperty_MultiSelection,     YBoolProperty,   true ) ); // read-only
	propSet.add( YWidget::propertySet() );

    }

    return propSet;
}


bool
YTree::setProperty( const string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if	    ( propertyName == YUIProperty_Value		)	return false; // Needs special handling
    else if ( propertyName == YUIProperty_CurrentItem 	)	return false; // Needs special handling
    else if ( propertyName == YUIProperty_CurrentBranch )	return false; // Needs special handling
    else if ( propertyName == YUIProperty_Items 	)	return false; // Needs special handling
    else if ( propertyName == YUIProperty_OpenItems 	)	return false; // Needs special handling
    else if ( propertyName == YUIProperty_SelectedItems )       return false; // Needs special handling
    else if ( propertyName == YUIProperty_Label		)	setLabel( val.stringVal() );
    else if ( propertyName == YUIProperty_IconPath 	)	setIconBasePath( val.stringVal() );

    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YTree::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if	    ( propertyName == YUIProperty_Value		)	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_CurrentItem 	)	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_CurrentBranch )	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_Items 	)	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_OpenItems 	)	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_Label		)	return YPropertyValue( label() );
    else if ( propertyName == YUIProperty_IconPath	)	return YPropertyValue( iconBasePath() );
    else if ( propertyName == YUIProperty_SelectedItems )       return YPropertyValue( YOtherProperty );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}

bool
YTree::hasMultiSelection() const
{
    return ! YSelectionWidget::enforceSingleSelection();
}


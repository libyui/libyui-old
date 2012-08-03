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

  File:		YMenuButton.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YMenuButton.h"
#include "YMenuItem.h"


struct YMenuButtonPrivate
{
    YMenuButtonPrivate()
	: nextSerialNo( 0 )
	{}

    int nextSerialNo;
};




YMenuButton::YMenuButton( YWidget * parent, const std::string & label )
    : YSelectionWidget( parent, label,
			false )	// enforceSingleSelection
    , priv( new YMenuButtonPrivate() )
{
    YUI_CHECK_NEW( priv );
}


YMenuButton::~YMenuButton()
{
    // NOP
}


void
YMenuButton::addItems( const YItemCollection & itemCollection )
{
    YSelectionWidget::addItems( itemCollection );
    resolveShortcutConflicts();
    rebuildMenuTree();
}


void
YMenuButton::addItem( YItem * item )
{
    YSelectionWidget::addItem( item );
    item->setIndex( ++(priv->nextSerialNo) );

    if ( item->hasChildren() )
	assignUniqueIndex( item->childrenBegin(), item->childrenEnd() );
}


void
YMenuButton::assignUniqueIndex( YItemIterator begin, YItemIterator end )
{
    for ( YItemIterator it = begin; it != end; ++it )
    {
	YItem * item = *it;

	item->setIndex( ++(priv->nextSerialNo) );

	if ( item->hasChildren() )
	    assignUniqueIndex( item->childrenBegin(), item->childrenEnd() );
    }
}


void
YMenuButton::deleteAllItems()
{
    YSelectionWidget::deleteAllItems();
    priv->nextSerialNo = 0;
}


YMenuItem *
YMenuButton::findMenuItem( int index )
{
    return findMenuItem( index, itemsBegin(), itemsEnd() );
}


YMenuItem *
YMenuButton::findMenuItem( int wantedIndex, YItemConstIterator begin, YItemConstIterator end )
{
    for ( YItemConstIterator it = begin; it != end; ++it )
    {
	YMenuItem * item = dynamic_cast<YMenuItem *> (*it);

	if ( item )
	{
	    if ( item->index() == wantedIndex )
		return item;

	    if ( item->hasChildren() )
	    {
		YMenuItem * result = findMenuItem( wantedIndex, item->childrenBegin(), item->childrenEnd() );

		if ( result )
		    return result;
	    }
	}
    }

    return 0;
}


void
YMenuButton::resolveShortcutConflicts()
{
    // TO DO
    // TO DO
    // TO DO

    // For every menu level, make sure keyboard shortcuts are unique within that menu level.
    // If necessary, change some of them (move the '&' character to some other character).


    // See YShortcutManager for more hints.
}


const YPropertySet &
YMenuButton::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property std::string	Label		Label on the menu button
	 * @property itemList		Items		All menu items and submenus
	 * @property std::string	IconPath	Base path for icons (on menu items)
	 */
	propSet.add( YProperty( YUIProperty_Label,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_Items,		YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_IconPath,		YStringProperty	 ) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YMenuButton::setProperty( const std::string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if      ( propertyName == YUIProperty_Label		)	setLabel( val.stringVal() );
    else if ( propertyName == YUIProperty_Items 	)	return false; // Needs special handling
    else if ( propertyName == YUIProperty_IconPath 	)	setIconBasePath( val.stringVal() );
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YMenuButton::getProperty( const std::string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if      ( propertyName == YUIProperty_Label		)	return YPropertyValue( label() );
    else if ( propertyName == YUIProperty_Items 	)	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_IconPath	)	return YPropertyValue( iconBasePath() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}

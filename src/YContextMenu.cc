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

  File:		YContextMenu.cc

  Author:	Thomas Goettlicher <tgoettlicher@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YContextMenu.h"
#include "YMenuItem.h"


struct YContextMenuPrivate
{
    YContextMenuPrivate()
	: nextSerialNo( 0 )
	{}
    
    int nextSerialNo;
};




YContextMenu::YContextMenu( )
    : YSelectionWidget( NULL, "test",
			false )	// enforceSingleSelection
    , priv( new YContextMenuPrivate() )
{
    YUI_CHECK_NEW( priv );
}


YContextMenu::~YContextMenu()
{
    // NOP
}


void
YContextMenu::addItems( const YItemCollection & itemCollection )
{
    YSelectionWidget::addItems( itemCollection );
    resolveShortcutConflicts();
    rebuildMenuTree();
}


void
YContextMenu::addItem( YItem * item )
{
    YSelectionWidget::addItem( item );
    item->setIndex( ++(priv->nextSerialNo) );

    if ( item->hasChildren() )
	assignUniqueIndex( item->childrenBegin(), item->childrenEnd() );
}


void
YContextMenu::assignUniqueIndex( YItemIterator begin, YItemIterator end )
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
YContextMenu::deleteAllItems()
{
    YSelectionWidget::deleteAllItems();
    priv->nextSerialNo = 0;
}


YMenuItem *
YContextMenu::findMenuItem( int index )
{
    return findMenuItem( index, itemsBegin(), itemsEnd() );
}


YMenuItem *
YContextMenu::findMenuItem( int wantedIndex, YItemConstIterator begin, YItemConstIterator end )
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
YContextMenu::resolveShortcutConflicts()
{
    // TO DO
    // TO DO
    // TO DO

    // For every menu level, make sure keyboard shortcuts are unique within that menu level.
    // If necessary, change some of them (move the '&' character to some other character).


    // See YShortcutManager for more hints.
}


const YPropertySet &
YContextMenu::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property string  	Label		Label on the menu button
	 * @property itemList	Items		All menu items and submenus
	 * @property string  	IconPath	Base path for icons (on menu items)
	 */
	propSet.add( YProperty( YUIProperty_Label,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_Items,		YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_IconPath,		YStringProperty	 ) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YContextMenu::setProperty( const string & propertyName, const YPropertyValue & val )
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
YContextMenu::getProperty( const string & propertyName )
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


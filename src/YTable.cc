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

  File:		YTable.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YTable.h"


struct YTablePrivate
{
    YTablePrivate( YTableHeader * header )
	: header( header )
	, keepSorting( false )
	, immediateMode( false )
    {
    }

    YTableHeader *	header;
    bool		keepSorting;
    bool		immediateMode;
};




YTable::YTable( YWidget * parent, YTableHeader * header )
    : YSelectionWidget( parent,
			"",	// label
			true )	// enforceSingleSelection
    , priv( new YTablePrivate( header ) )
{
    YUI_CHECK_PTR( header );
    YUI_CHECK_NEW( priv   );

    setDefaultStretchable( YD_HORIZ, true );
    setDefaultStretchable( YD_VERT,  true );
}


YTable::~YTable()
{
    if ( priv->header )
	delete priv->header;
}


void
YTable::setTableHeader( YTableHeader * newHeader )
{
    YUI_CHECK_PTR( newHeader );

    if ( priv->header->columns() != newHeader->columns() )
	deleteAllItems();

    delete priv->header;
    priv->header = newHeader;
}


int
YTable::columns() const
{
    return priv->header->columns();
}


bool
YTable::hasColumn( int column ) const
{
    return priv->header->hasColumn( column );
}


string
YTable::header( int column ) const
{
    return priv->header->header( column );
}


YAlignmentType
YTable::alignment( int column ) const
{
    return priv->header->alignment( column );
}


bool
YTable::immediateMode() const
{
    return priv->immediateMode;
}


void
YTable::setImmediateMode( bool immediateMode )
{
    priv->immediateMode = immediateMode;

    if ( immediateMode )
	setNotify( true );
}


bool
YTable::keepSorting() const
{
    return priv->keepSorting;
}


void
YTable::setKeepSorting( bool keepSorting )
{
    priv->keepSorting = keepSorting;
}



const YPropertySet &
YTable::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property itemID  	Value		The currently selected item
	 * @property itemID  	CurrentItem	The currently selected item
	 * @property itemList	Items		All items
	 * @property string	Cell		One cell (one column of one item)
	 * @property integer	Cell		(ChangeWidget only) One cell as integer
	 * @property `icon(...)	Cell		Icon for one one cell
	 * @property string	Item		Alias for Cell
	 * @property string	Item		QueryWidget only: Return one complete item
	 * @property string  	IconPath	Base path for icons
	 */
	propSet.add( YProperty( YUIProperty_Value,		YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_CurrentItem,	YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_Items,		YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_Cell,		YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_Item,		YOtherProperty	 ) );
	propSet.add( YProperty( YUIProperty_IconPath,		YStringProperty	 ) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YTable::setProperty( const string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if	    ( propertyName == YUIProperty_Value		)	return false; // Needs special handling
    else if ( propertyName == YUIProperty_CurrentItem 	)	return false; // Needs special handling
    else if ( propertyName == YUIProperty_Items 	)	return false; // Needs special handling
    else if ( propertyName == YUIProperty_Cell		)	return false; // Needs special handling
    else if ( propertyName == YUIProperty_Item 		)	return false; // Needs special handling
    else if ( propertyName == YUIProperty_IconPath 	)	setIconBasePath( val.stringVal() );
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YTable::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if	    ( propertyName == YUIProperty_Value		)	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_CurrentItem 	)	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_Items 	)	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_Cell		)	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_Item 		)	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_IconPath	)	return YPropertyValue( iconBasePath() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}

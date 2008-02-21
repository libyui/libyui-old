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

  File:		YTableItem.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include "YTableItem.h"
#include "YUIException.h"


YTableItem::YTableItem()
    : YItem( "" )
{
    // NOP
}


YTableItem::YTableItem( const string & label_0,
			const string & label_1,
			const string & label_2,
			const string & label_3,
			const string & label_4,
			const string & label_5,
			const string & label_6,
			const string & label_7,
			const string & label_8,
			const string & label_9 )
    : YItem( "" )
{
    vector<string> labels;
    labels.reserve(10); // slight optimization
    labels.push_back( label_0 );
    labels.push_back( label_1 );
    labels.push_back( label_2 );
    labels.push_back( label_3 );
    labels.push_back( label_4 );
    labels.push_back( label_5 );
    labels.push_back( label_6 );
    labels.push_back( label_7 );
    labels.push_back( label_8 );
    labels.push_back( label_9 );

    //
    // Find the last non-empty label
    //

    unsigned lastLabel = labels.size() - 1;

    while ( labels[ lastLabel ].empty() && --lastLabel > 0 )
    {}

    //
    // Create cells
    //

    for ( unsigned i = 0; i <= lastLabel; ++i )
    {
	addCell( labels[i] );
    }
}



YTableItem::~YTableItem()
{
    deleteCells();
}


void
YTableItem::deleteCells()
{
    YTableCellIterator it = cellsBegin();

    while ( it != cellsEnd() )
    {
	YTableCell * cell = *it;
	++it;
	delete cell;
    }

    _cells.clear();
}


void
YTableItem::addCell( YTableCell * cell )
{
    YUI_CHECK_PTR( cell );
    _cells.push_back( cell );

    cell->reparent( this, _cells.size() - 1 );
}


void
YTableItem::addCell( const string & label, const string & iconName )
{
    YTableCell * cell = new YTableCell( label, iconName );
    YUI_CHECK_NEW( cell );

    addCell( cell );
}


bool
YTableItem::hasCell( int index ) const
{
    return index >= 0 && (unsigned) index < _cells.size();
}


const YTableCell *
YTableItem::cell( int index ) const
{
    return hasCell( index ) ?
	_cells[ index ] : 0;
}


YTableCell *
YTableItem::cell( int index )
{
    return hasCell( index ) ?
	_cells[ index ] : 0;
}


string
YTableItem::label( int index ) const
{
    return hasCell( index ) ? _cells[ index ]->label() : "";
}


string
YTableItem::iconName( int index ) const
{
    return hasCell( index ) ? _cells[ index ]->iconName() : "";
}


bool
YTableItem::hasIconName( int index ) const
{
    return hasCell( index ) ? _cells[ index ]->hasIconName() : false;
}





void YTableCell::reparent( YTableItem * parent, int column )
{
    YUI_CHECK_PTR( parent );

    if ( _parent && _parent != parent && column != column )
	YUI_THROW( YUIException( string( "Cannot reparent YTableCell \"" )
				 + _label
				 + "to different parent." ) );
    _parent = parent;
    _column = column;
}

/****************************************************************************

Copyright (c) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, contact Novell, Inc.

To contact Novell about this file by physical or electronic mail,
you may find current contact information at www.novell.com

****************************************************************************

  File:		YCPTableItemWriter.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <ycp/YCPVoid.h>
#include "YCPTableItemWriter.h"
#include <yui/YUISymbols.h>


YCPList
YCPTableItemWriter::itemList( YItemConstIterator begin, YItemConstIterator end )
{
    YCPList itemList;

    for ( YItemConstIterator it = begin; it != end; ++it )
    {
	const YTableItem * item = dynamic_cast<const YTableItem *> (*it);

	if ( item )
	{
	    itemList->add( itemTerm( item ) );
	}
    }

    return itemList;
}


YCPValue
YCPTableItemWriter::itemTerm( const YTableItem * item )
{
    if ( ! item )
	return YCPVoid();

    YCPTerm itemTerm( YUISymbol_item );	// `item()

    const YCPTableItem * ycpItem = dynamic_cast<const YCPTableItem *> (item);

    if ( ycpItem && ycpItem->hasId() )
    {
	YCPTerm idTerm( YUISymbol_id );	// `id()
	idTerm->add( ycpItem->id() );
	itemTerm->add( idTerm );
    }

    for ( YTableCellConstIterator it = item->cellsBegin(); it != item->cellsEnd(); ++it )
	itemTerm->add( cellValue( *it ) );

    return itemTerm;
}


YCPValue
YCPTableItemWriter::cellValue( const YTableCell * cell )
{
    if ( ! cell )
	return YCPVoid();

    if ( cell->hasIconName() )
    {
	YCPTerm iconTerm( YUISymbol_icon );		// `icon()
	iconTerm->add( YCPString( cell->iconName() ) );	// `icon("myicon.png")

	YCPTerm cellTerm( YUISymbol_cell );		// `cell()
	cellTerm->add( iconTerm );			// `cell(`icon("myicon.png"))
	cellTerm->add( YCPString( cell->label() ) );	// `cell(`icon("myicon.png"), "mylabel")

	return cellTerm;
    }
    else // no icon
    {
	return YCPString( cell->label() );		// "mylabel"
    }
}


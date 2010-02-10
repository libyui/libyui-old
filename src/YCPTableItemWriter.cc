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

  File:		YCPTableItemWriter.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <ycp/YCPVoid.h>
#include "YCPTableItemWriter.h"
#include "YUISymbols.h"


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


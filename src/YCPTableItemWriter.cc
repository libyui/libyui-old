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


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

  File:		YCPTreeItemWriter.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <ycp/YCPVoid.h>
#include <ycp/YCPBoolean.h>
#include "YCPTreeItemWriter.h"
#include "YUISymbols.h"


YCPList
YCPTreeItemWriter::itemList( YItemConstIterator begin, YItemConstIterator end )
{
    YCPList itemList;

    for ( YItemConstIterator it = begin; it != end; ++it )
    {
	const YTreeItem * item = dynamic_cast<const YTreeItem *> (*it);

	if ( item )
	{
	    itemList->add( itemTerm( item ) );
	}
    }

    return itemList;
}


YCPValue
YCPTreeItemWriter::itemTerm( const YTreeItem * item )
{
    if ( ! item )
	return YCPVoid();

    YCPTerm itemTerm( YUISymbol_item );	// `item()

    const YCPTreeItem * ycpItem = dynamic_cast<const YCPTreeItem *> (item);

    if ( ycpItem && ycpItem->hasId() )
    {
	YCPTerm idTerm( YUISymbol_id );			// `id()
	idTerm->add( ycpItem->id() );
	itemTerm->add( idTerm );
    }

    if ( item->hasIconName() )
    {
	YCPTerm iconTerm( YUISymbol_icon );		// `icon()
	iconTerm->add( YCPString( item->iconName() ) );
	itemTerm->add( iconTerm );
    }

    itemTerm->add( YCPString( item->label() ) );	// label

    if ( item->isOpen() )				// isOpen
	itemTerm->add( YCPBoolean( item->isOpen() ) );

    if ( item->hasChildren() )				// subItemList
	itemTerm->add( itemList( item->childrenBegin(), item->childrenEnd() ) );

    return itemTerm;
}



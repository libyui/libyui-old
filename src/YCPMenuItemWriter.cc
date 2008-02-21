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

  File:		YCPMenuItemWriter.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <ycp/YCPVoid.h>
#include <ycp/YCPBoolean.h>
#include "YCPMenuItemWriter.h"
#include "YUISymbols.h"


YCPList
YCPMenuItemWriter::itemList( YItemConstIterator begin, YItemConstIterator end )
{
    YCPList itemList;

    for ( YItemConstIterator it = begin; it != end; ++it )
    {
	const YMenuItem * item = dynamic_cast<const YMenuItem *> (*it);

	if ( item )
	{
	    itemList->add( itemTerm( item ) );
	}
    }

    return itemList;
}


YCPValue
YCPMenuItemWriter::itemTerm( const YMenuItem * item )
{
    if ( ! item )
	return YCPVoid();

    YCPTerm itemTerm( YUISymbol_item );	// `item()

    const YCPMenuItem * ycpItem = dynamic_cast<const YCPMenuItem *> (item);

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

    if ( item->hasChildren() )				// subItemList
	itemTerm->add( itemList( item->childrenBegin(), item->childrenEnd() ) );

    return itemTerm;
}



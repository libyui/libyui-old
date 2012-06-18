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

  File:		YCPItemWriter.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <ycp/YCPVoid.h>
#include <ycp/YCPBoolean.h>
#include "YCPItemWriter.h"
#include <yui/YUISymbols.h>


YCPList
YCPItemWriter::itemList( YItemConstIterator begin, YItemConstIterator end )
{
    YCPList itemList;

    for ( YItemConstIterator it = begin; it != end; ++it )
    {
	const YItem * item = dynamic_cast<const YItem *> (*it);

	if ( item )
	{
	    itemList->add( itemTerm( item ) );
	}
    }

    return itemList;
}


YCPValue
YCPItemWriter::itemTerm( const YItem * item )
{
    if ( ! item )
	return YCPVoid();

    YCPTerm itemTerm( YUISymbol_item );	// `item()

    const YCPItem * ycpItem = dynamic_cast<const YCPItem *> (item);

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

    if ( item->selected() )				// isSelected
	itemTerm->add( YCPBoolean( item->selected() ) );

    return itemTerm;
}



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

  File:		YCPItemParser.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <ycp/YCPTerm.h>
#include <ycp/YCPBoolean.h>

#include "YCPItemParser.h"
#include "YCP_UI_Exception.h"
#include <yui/YUISymbols.h>


YItemCollection
YCPItemParser::parseItemList( const YCPList & itemList )
{
    YItemCollection itemCollection;
    itemCollection.reserve( itemList.size() );

    try
    {
	for ( int i=0; i < itemList->size(); i++ )
	{
	    YCPItem * item = parseItem( itemList->value(i) );
	    itemCollection.push_back( item );
	}
    }
    catch ( YUIException & exception )
    {
	// Delete all items created so far
	
	YItemIterator it = itemCollection.begin();

	while ( it != itemCollection.end() )
	{
	    YItem * item = *it;
	    ++it;
	    delete item;
	}

	throw;
    }

    return itemCollection;
}



YCPItem *
YCPItemParser::parseItem( const YCPValue & rawItem )
{
    YCPItem * item = 0;

    if ( rawItem->isString() )		// Simple case: just a string
    {
	YCPString label = rawItem->asString();
	item = new YCPItem( label,	// The real label
			    label,	// Use the label as ID, too
			    false );	// Not initially selected
    }
    else				// `item(...)
    {
	if ( rawItem->isTerm() &&
	     rawItem->asTerm()->name() == YUISymbol_item )	// `item(...)
	{
	    item = parseItem( rawItem->asTerm() );

	}
	else	// not `item(...)
	{
	    YUI_THROW( YCPDialogSyntaxErrorException( "Expected `item(...)", rawItem ) );
	}
    }

    return item;
}


YCPItem *
YCPItemParser::parseItem( const YCPTerm & itemTerm )
{
    YCPValue	id	 = YCPNull();
    YCPString	iconName = YCPNull();
    YCPString	label	 = YCPNull();
    YCPBoolean  selected = YCPNull();
    
    const char * usage =
	"Expected: `item(`id(`myID), `icon(\"MyIcon.png\"), \"MyItemText\", boolean selected )";
    

    for ( int i=0; i < itemTerm->size(); i++ )
    {
	YCPValue arg = itemTerm->value( i );

	if ( arg->isTerm() )	// `id(), `icon()
	{
	    YCPTerm term = arg->asTerm();

	    if ( term->size() != 1 )		// Both `id() and `icon() have 1 argument
		YUI_THROW( YCPDialogSyntaxErrorException( usage, itemTerm ) );

	    if ( term->name() == YUISymbol_id		// `id(...)
		 && id.isNull() )			// and don't have an ID yet
	    {
		id = term->value(0);
	    }
	    else if ( term->name() == YUISymbol_icon	// `icon(...)
		      && term->value(0)->isString()	// with a string argument
		      && iconName.isNull() )		// and don't have an icon name yet
	    {
		iconName = term->value(0)->asString();
	    }
	    else
	    {
		YUI_THROW( YCPDialogSyntaxErrorException( usage, itemTerm ) );
	    }
	}
	else if ( arg->isString()		// label (the user-visible text)
		  && label.isNull() )		// and don't have a label yet
	{
	    label = arg->asString();
	}
	else if ( arg->isBoolean() 		// "selected" flag
		  && selected.isNull() )	// and don't have a "selected" flag yet
	{
	    selected = arg->asBoolean();
	}
	else
	{
	    YUI_THROW( YCPDialogSyntaxErrorException( usage, itemTerm ) );
	}
    }

    if ( label.isNull() )		// the label is required
	YUI_THROW( YCPDialogSyntaxErrorException( usage, itemTerm ) );

    if ( iconName.isNull() )
	iconName = YCPString( "" );

    if ( id.isNull() )			// no `id() ?
	id = label;			// use the label instead

    if ( selected.isNull() )		// "selected" not specified?
	selected = YCPBoolean( false );	// use "not selected" (false) as default

    YCPItem * item = new YCPItem( label, id, iconName, selected->value() );
    YUI_CHECK_NEW( item );

    return item;
}

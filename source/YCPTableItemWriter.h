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

  File:		YCPTableItemWriter.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPTableItemWriter_h
#define YCPTableItemWriter_h

#include <ycp/YCPList.h>
#include <ycp/YCPTerm.h>
#include <ycp/YCPValue.h>

#include "YCPTableItem.h"


/**
 * Writer for table item lists
 **/
class YCPTableItemWriter
{
public:

    /**
     * Create a YCPList from a table item collection:
     *
     *     [
     *         `item(`id(`myID1 ), "Label1", "Label2", "Label3" ),
     *         `item(`id(`myID2 ), "Label1", `cell("Label2"), "Label3" ),
     *         `item(`id(`myID3 ), "Label1", `cell(`icon( "icon2.png"), "Label2" ), "Label3" )
     *         `item(`id(`myID4 ), "Label1", `cell( "Label2", `icon( "icon2.png")), "Label3" )
     *     ]
     
     * Note that the simple version "Label4" (without `item() and `id()) is
     * never returned here since if no ID is explicitly specified, the label
     * itself is used as the ID, so every item always has an ID, so the `id()
     * term and thus the `item() term have to be used.
     **/
    static YCPList itemList( YItemConstIterator begin, YItemConstIterator end );


    /**
     * Create a YCPTerm for one table item:
     *
     *         `item(`id(`myID1 ), "Label1", "Label2", "Label3" )
     *         `item(`id(`myID2 ), "Label1", `cell( "Label2" ), "Label3" )
     *         `item(`id(`myID3 ), "Label1", `cell(`icon( "icon2.png" ), "Label2" ), "Label3" )
     *         `item(`id(`myID4 ), "Label1", `cell( "Label2", `icon( "icon2.png")), "Label3" )
     *
     * `cell() will be created only if a cell has an icon.
     **/
    static YCPValue itemTerm( const YTableItem * item );

    /**
     * Create a YCPValue for one table cell: A YCPString if a cell has only a
     * label and no icon, and a YCPTerm with `cell() otherwise.
     *
     *		"Label"
     *         `cell( `icon( "icon.png"), "Label" )
     **/
    static YCPValue cellValue( const YTableCell * cell );
};


#endif // YCPTableItemWriter_h

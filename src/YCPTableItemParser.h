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

  File:		YCPTableItemParser.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPTableItemParser_h
#define YCPTableItemParser_h

#include <ycp/YCPTerm.h>
#include "YCPTableItem.h"


/**
 * Parser for table item lists
 **/
class YCPTableItemParser
{
public:

    /**
     * Parse a table item list:
     *
     *     [
     *         `item(`id(`myID1 ), "Label1", "Label2", "Label3" ),
     *         `item(`id(`myID1 ), "Label1", `cell("Label2"), "Label3" ),
     *         `item(`id(`myID2 ), "Label1", `cell(`icon( "icon2.png"), "Label2" ), "Label3" ),
     *         `item(`id(`myID2 ), "Label1", `cell( "Label2", `icon( "icon2.png")), "Label3" )
     *     ]
     *
     * Return a list of newly created YTableItem-derived objects.
     *
     * This function throws exceptions if there are syntax errors.
     **/
    static YItemCollection parseTableItemList( const YCPList & ycpItemList );


protected:

    /**
     * Parse an item term:
     *
     *         `item(`id(`myID1 ), "Label1", "Label2", "Label3" )
     *         `item(`id(`myID1 ), "Label1", `cell( "Label2" ), "Label3" )
     *         `item(`id(`myID2 ), "Label1", `cell(`icon( "icon2.png" ), "Label2" ), "Label3" )
     *         `item(`id(`myID2 ), "Label1", `cell( "Label2", `icon( "icon2.png")), "Label3" )
     *
     * `cell() is optional if there is only a label and no icon for that cell.
     *
     * This function throws exceptions if there are syntax errors.
     **/
    static YCPTableItem * parseTableItem( const YCPTerm & itemTerm );

    /**
     * Parse a cell term and add a YTableCell to the parent YCPTableItem:
     *
     *         `cell( "Label" )
     *         `cell( `icon( "icon.png"), "Label" )
     *         `cell( "Label", `icon( "icon.png" ) )
     **/
    static void parseTableCell( YCPTableItem * parent, const YCPTerm & cellTerm );
};


#endif // YCPTableItemParser_h

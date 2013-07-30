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


  File:		YCPTreeItemParser.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPTreeItemParser_h
#define YCPTreeItemParser_h

#include <ycp/YCPTerm.h>
#include "YCPTreeItem.h"


/**
 * Parser for tree item lists
 **/
class YCPTreeItemParser
{
public:

    /**
     * Parse a tree item list:
     *
     *     [
     *         `item(`id( `myID1 ), "Label1" ),
     *         `item(`id( `myID2 ), `icon( "icon2.png"), "Label2", true ),
     *         `item(`id( `myID3 ), `icon( "icon3.png"), "Label3", true, [ subItemList ] ),
     *         "Label4"
     *     ]
     *
     * Return a list of newly created YItem-derived objects.
     *
     * This function throws exceptions if there are syntax errors.
     **/
    static YItemCollection parseTreeItemList( const YCPList & ycpItemList );


    /**
     * Parse one item and create a YCPTreeItem from it.
     *
     * This function throws exceptions if there are syntax errors.
     **/
    static YCPTreeItem * parseTreeItem( YCPTreeItem * parent, const YCPValue & item );

protected:

    /**
     * Parse an item term:
     *
     *         `item(`id( `myID1 ), "Label1" )
     *         `item(`id( `myID2 ), `icon( "icon2.png"), "Label2", true )
     *         `item(`id( `myID3 ), `icon( "icon3.png"), "Label3", true, [ subItemList ] )
     *         "Label4"
     *
     * Everything except the label is optional.
     *
     * This function throws exceptions if there are syntax errors.
     **/
    static YCPTreeItem * parseTreeItem( YCPTreeItem * parent, const YCPTerm & itemTerm );

    
};


#endif // YCPTreeItemParser_h

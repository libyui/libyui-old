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


  File:		YCPTreeItemWriter.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPTreeItemWriter_h
#define YCPTreeItemWriter_h

#include <ycp/YCPList.h>
#include <ycp/YCPTerm.h>
#include <ycp/YCPValue.h>

#include "YCPTreeItem.h"


/**
 * Writer for tree item lists
 **/
class YCPTreeItemWriter
{
public:

    /**
     * Create a YCPList from a tree item collection:
     *
     *     [
     *         `item(`id( `myID1 ), "Label1" ),
     *         `item(`id( `myID2 ), `icon( "icon2.png"), "Label2", true ),
     *         `item(`id( `myID3 ), `icon( "icon3.png"), "Label3", true, [ subItemList ] ),
     *	       `item( "Label4" )
     *     ]
     **/
    static YCPList itemList( YItemConstIterator begin, YItemConstIterator end );


    /**
     * Create a YCPTerm for one tree item:
     *
     *         `item(`id( `myID1 ), "Label1" ),
     *         `item(`id( `myID2 ), `icon( "icon2.png"), "Label2", true ),
     *         `item(`id( `myID3 ), `icon( "icon3.png"), "Label3", true, [ subItemList ] )
     *	       `item( "Label4" )
     **/
    static YCPValue itemTerm( const YTreeItem * item );

};


#endif // YCPTreeItemWriter_h

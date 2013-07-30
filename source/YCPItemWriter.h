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

  File:		YCPItemWriter.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPItemWriter_h
#define YCPItemWriter_h

#include <ycp/YCPList.h>
#include <ycp/YCPTerm.h>
#include <ycp/YCPValue.h>

#include "YCPItem.h"


/**
 * Writer for SelectionBox, ComboBox, MultiSelectionBox item lists
 **/
class YCPItemWriter
{
public:

    /**
     * Create a YCPList from an item collection:
     *
     *     [
     *         `item(`id( `myID1 ), "Label1" ),
     *         `item(`id( `myID2 ), "Label2", true ),
     *         `item(`id( `myID3 ), `icon( "icon3.png"), "Label3", true )
     *     ]
     *
     * Note that the simple version "Label4" (without `item() and `id()) is
     * never returned here since if no ID is explicitly specified, the label
     * itself is used as the ID, so every item always has an ID, so the `id()
     * term and thus the `item() term have to be used.
     **/
    static YCPList itemList( YItemConstIterator begin, YItemConstIterator end );


    /**
     * Create a YCPTerm for one item:
     *
     *         `item(`id( `myID1 ), "Label1" )
     *         `item(`id( `myID2 ), "Label2", true )
     *         `item(`id( `myID3 ), `icon( "icon3.png"), "Label3", true )
     **/
    static YCPValue itemTerm( const YItem * item );

};


#endif // YCPItemWriter_h

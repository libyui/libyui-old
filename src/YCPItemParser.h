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

#ifndef YCPItemParser_h
#define YCPItemParser_h

#include <ycp/YCPTerm.h>
#include "YCPItem.h"


/**
 * Parser for SelectionBox, ComboBox, MultiSelectionBox item lists
 **/
class YCPItemParser
{
public:

    /**
     * Parse an item list:
     *
     *     [
     *         `item(`id( `myID1 ), "Label1" ),
     *         `item(`id( `myID2 ), `icon( "icon2.png"), "Label2", true ),
     *         "Label3"
     *     ]
     *
     * Return a list of newly created YItem-derived objects.
     *
     * This function throws exceptions if there are syntax errors.
     **/
    static YItemCollection parseItemList( const YCPList & ycpItemList );


    /**
     * Parse one item and create a YCPItem from it.
     *
     * This function throws exceptions if there are syntax errors.
     **/
    static YCPItem   * parseItem( const YCPValue & item );

protected:

    /**
     * Parse an item term:
     *
     *         `item(`id( `myID1 ), "Label1" )
     *         `item(`id( `myID2 ), `icon( "icon2.png"), "Label2", true )
     *
     * Everything except the label is optional.
     *
     * This function throws exceptions if there are syntax errors.
     **/
    static YCPItem * parseItem( const YCPTerm & itemTerm );

    
};


#endif // YCPItemParser_h

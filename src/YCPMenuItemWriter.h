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

  File:		YCPMenuItemWriter.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPMenuItemWriter_h
#define YCPMenuItemWriter_h

#include <ycp/YCPList.h>
#include <ycp/YCPTerm.h>
#include <ycp/YCPValue.h>

#include "YCPMenuItem.h"


/**
 * Writer for menu item lists
 **/
class YCPMenuItemWriter
{
public:

    /**
     * Create a YCPList from a tree item collection:
     *
     *     [
     *         `item(`id( `myID1 ), "Label1" ),
     *         `item(`id( `myID2 ), `icon( "icon2.png"), "Label2", ),
     *         `item(`id( `myID3 ), `icon( "icon3.png"), "Label3", [ subMenuList ] ),
     *     ]
     **/
    static YCPList itemList( YItemConstIterator begin, YItemConstIterator end );


    /**
     * Create a YCPTerm for one tree item:
     *
     *         `item(`id( `myID1 ), "Label1" )
     *         `item(`id( `myID2 ), `icon( "icon2.png"), "Label2", )
     *         `item(`id( `myID3 ), `icon( "icon3.png"), "Label3", [ subMenuList ] )
     **/
    static YCPValue itemTerm( const YMenuItem * item );

};


#endif // YCPMenuItemWriter_h

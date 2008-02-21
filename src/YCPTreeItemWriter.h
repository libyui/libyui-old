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

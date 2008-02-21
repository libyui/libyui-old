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

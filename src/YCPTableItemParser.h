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

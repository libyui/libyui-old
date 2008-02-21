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

  File:		YCPMenuItemParser.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPMenuItemParser_h
#define YCPMenuItemParser_h

#include <ycp/YCPTerm.h>
#include "YCPMenuItem.h"


/**
 * Parser for menu item lists
 **/
class YCPMenuItemParser
{
public:

    /**
     * Parse a menu item list:
     *
     *     [
     *         `item(`id( `myID1 ), "Label1" ),
     *         `item(`id( `myID2 ), `icon( "icon2.png"), "Label2", ),
     *         `menu(`id( `myID3 ), `icon( "icon3.png"), "Label3", [ subMenuList ] ),
     *         `item(`id( `myID3 ), `icon( "icon3.png"), "Label3", [ subMenuList ] ),
     *     ]
     *
     * Return a list of newly created YItem-derived objects.
     *
     * This function throws exceptions if there are syntax errors.
     **/
    static YItemCollection parseMenuItemList( const YCPList & ycpItemList );


    /**
     * Parse one item and create a YCPMenuItem from it.
     *
     * This function throws exceptions if there are syntax errors.
     **/
    static YCPMenuItem * parseMenuItem( YCPMenuItem * parent, const YCPValue & item );

protected:

    /**
     * Parse an item term:
     *
     *         `item(`id( `myID1 ), "Label1" )
     *         `item(`id( `myID2 ), `icon( "icon2.png"), "Label2", )
     *         `menu(`id( `myID3 ), `icon( "icon3.png"), "Label3", [ subMenuList ] )
     *         `item(`id( `myID3 ), `icon( "icon3.png"), "Label3", [ subMenuList ] )
     *
     * Everything but the label is optional. If no ID is specified, the label
     * will be used as ID, which might not be very useful if labels are translated.
     *
     * This function throws exceptions if there are syntax errors.
     **/
    static YCPMenuItem * parseMenuItem( YCPMenuItem * parent, const YCPTerm & itemTerm );

    
};


#endif // YCPMenuItemParser_h

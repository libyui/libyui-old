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

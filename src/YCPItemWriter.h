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

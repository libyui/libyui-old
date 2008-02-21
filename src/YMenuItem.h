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

  File:		YMenuItem.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YMenuItem_h
#define YMenuItem_h

#include "YTreeItem.h"

using std::string;
using std::vector;


/**
 * Item class for menu items.
 **/
class YMenuItem: public YTreeItem
{
public:
    /**
     * Constructors for toplevel items.
     **/
    YMenuItem( const string &	label )
	: YTreeItem( label )
	{}

    YMenuItem( const string & 	label,
	       const string & 	iconName )
	: YTreeItem( label, iconName )
	{}

    /**
     * Constructors for items that have a parent item.
     *
     * They will automatically register this item with the parent item. The
     * parent assumes ownership of this item and will delete it in its (the
     * parent's) destructor.
     **/
    YMenuItem( YMenuItem * 	parent,
	       const string & 	label )
	: YTreeItem( parent, label )
	{}

    YMenuItem( YMenuItem * 	parent,
	       const string & 	label,
	       const string & 	iconName )
	: YTreeItem( parent, label, iconName )
	{}

    /**
     * Destructor.
     *
     * This will delete all children.
     **/
    virtual ~YMenuItem() {}


    /**
     * Returns this item's parent item or 0 if it is a toplevel item.
     **/
    YMenuItem * parent() const
	{ return dynamic_cast<YMenuItem *> ( YTreeItem::parent() ); }


private:

    // Disable unwanted base class methods
    
    bool isOpen() const  { return false; }
    void setOpen( bool ) {}
};


#endif // YMenuItem_h

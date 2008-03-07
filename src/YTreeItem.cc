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

  File:		YTreeItem.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include "YTreeItem.h"


YTreeItem::YTreeItem( const string &	label,
		      bool 		isOpen )
    : YItem( label )
    , _parent( 0 )
    , _isOpen( isOpen )
{
}


YTreeItem::YTreeItem( const string &	label,
		      const string & 	iconName,
		      bool 		isOpen )
    : YItem( label, iconName )
    , _parent( 0 )
    , _isOpen( isOpen )
{
}


YTreeItem::YTreeItem( YTreeItem *	parent,
		      const string &	label,
		      bool 		isOpen )
    : YItem( label )
    , _parent( parent )
    , _isOpen( isOpen )
{
    if ( parent )
	parent->addChild( this );
}


YTreeItem::YTreeItem( YTreeItem *	parent,
		      const string & 	label,
		      const string & 	iconName,
		      bool 		isOpen )
    : YItem( label, iconName )
    , _parent( parent )
    , _isOpen( isOpen )
{
    if ( parent )
	parent->addChild( this );
}


YTreeItem::~YTreeItem()
{
    deleteChildren();
}


void YTreeItem::addChild( YItem * child )
{
    _children.push_back( child );
}


void YTreeItem::deleteChildren()
{
    YItemIterator it = childrenBegin();

    while ( it != childrenEnd() )
    {
	YItem * child = *it;
	++it;
	delete child;
    }

    _children.clear();
}


bool YTreeItem::isOpen() const
{
    return hasChildren() ? _isOpen : false;
}


void YTreeItem::setOpen( bool open )
{
    _isOpen = open;
}


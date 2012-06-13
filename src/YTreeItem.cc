/*
  |****************************************************************************
  |
  | Copyright (c) 2000 - 2012 Novell, Inc.
  | All Rights Reserved.
  |
  | This program is free software; you can redistribute it and/or
  | modify it under the terms of version 2 of the GNU General Public License as
  | published by the Free Software Foundation.
  |
  | This program is distributed in the hope that it will be useful,
  | but WITHOUT ANY WARRANTY; without even the implied warranty of
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
  | GNU General Public License for more details.
  |
  | You should have received a copy of the GNU General Public License
  | along with this program; if not, contact Novell, Inc.
  |
  | To contact Novell about this file by physical or electronic mail,
  | you may find current contact information at www.novell.com
  |
  |****************************************************************************
*/



 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////   __/\\\\\\_____________/\\\__________/\\\________/\\\___/\\\________/\\\___/\\\\\\\\\\\_           ////
 ////    _\////\\\____________\/\\\_________\///\\\____/\\\/___\/\\\_______\/\\\__\/////\\\///__          ////
 ////     ____\/\\\______/\\\__\/\\\___________\///\\\/\\\/_____\/\\\_______\/\\\______\/\\\_____         ////
 ////      ____\/\\\_____\///___\/\\\_____________\///\\\/_______\/\\\_______\/\\\______\/\\\_____        ////
 ////       ____\/\\\______/\\\__\/\\\\\\\\\_________\/\\\________\/\\\_______\/\\\______\/\\\_____       ////
 ////        ____\/\\\_____\/\\\__\/\\\////\\\________\/\\\________\/\\\_______\/\\\______\/\\\_____      ////
 ////         ____\/\\\_____\/\\\__\/\\\__\/\\\________\/\\\________\//\\\______/\\\_______\/\\\_____     ////
 ////          __/\\\\\\\\\__\/\\\__\/\\\\\\\\\_________\/\\\_________\///\\\\\\\\\/_____/\\\\\\\\\\\_    ////
 ////           _\/////////___\///___\/////////__________\///____________\/////////______\///////////__   ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                 widget abstraction library providing Qt, GTK and ncurses frontends                  ////
 ////                                                                                                     ////
 ////                                   3 UIs for the price of one code                                   ////
 ////                                                                                                     ////
 ////                                       ***  core library  ***                                        ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                              (C) SUSE Linux GmbH    ////
 ////                                                                                                     ////
 ////                                                              libYUI-AsciiArt (C) 2012 Björn Esser   ////
 ////                                                                                                     ////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-/

  File:		YTreeItem.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include "YTreeItem.h"


YTreeItem::YTreeItem( const std::string &	label,
		      bool			isOpen )
    : YItem( label )
    , _parent( 0 )
    , _isOpen( isOpen )
{
}


YTreeItem::YTreeItem( const std::string &	label,
		      const std::string &	iconName,
		      bool			isOpen )
    : YItem( label, iconName )
    , _parent( 0 )
    , _isOpen( isOpen )
{
}


YTreeItem::YTreeItem( YTreeItem *		parent,
		      const std::string &	label,
		      bool			isOpen )
    : YItem( label )
    , _parent( parent )
    , _isOpen( isOpen )
{
    if ( parent )
	parent->addChild( this );
}


YTreeItem::YTreeItem( YTreeItem *		parent,
		      const std::string &	label,
		      const std::string &	iconName,
		      bool			isOpen )
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

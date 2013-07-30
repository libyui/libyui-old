/****************************************************************************

Copyright (c) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, contact Novell, Inc.

To contact Novell about this file by physical or electronic mail,
you may find current contact information at www.novell.com

****************************************************************************

  File:		YCPTreeItem.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPTreeItem_h
#define YCPTreeItem_h

#include <ycp/YCPValue.h>
#include <ycp/YCPString.h>
#include <yui/YTreeItem.h>


/**
 * Tree item class with YCPValue IDs.
 *
 * Similar to YCPItem, but with YTreeItem as base class. There should be an
 * elegant way to do this with templates, but the different constructor
 * arguments make this difficult.
 **/
class YCPTreeItem: public YTreeItem
{
public:

    /**
     * Constructors
     **/
    YCPTreeItem( const YCPString & 	label,
		 const YCPValue  & 	id,
		 bool			isOpen )
	: YTreeItem( label->value(), isOpen )
	, _id( id )
	{}

    YCPTreeItem( const YCPString & 	label,
		 const YCPValue  & 	id,
		 const YCPString & 	iconName,
		 bool			isOpen )
	: YTreeItem( label->value(), iconName->value(), isOpen )
	, _id( id )
	{}

    YCPTreeItem( YCPTreeItem *		parent,
		 const YCPString & 	label,
		 const YCPValue  & 	id,
		 bool			isOpen )
	: YTreeItem( parent, label->value(), isOpen )
	, _id( id )
	{}

    YCPTreeItem( YCPTreeItem *		parent,
		 const YCPString & 	label,
		 const YCPValue  & 	id,
		 const YCPString & 	iconName,
		 bool			isOpen )
	: YTreeItem( parent, label->value(), iconName->value(), isOpen )
	, _id( id )
	{}

    /**
     * Destructor.
     **/
    virtual ~YCPTreeItem()
	{}

    /**
     * Return 'true' if this item has an ID.
     **/
    bool hasId() const { return ! _id.isNull() && ! _id->isVoid(); }

    /**
     * Return this item's ID.
     **/
    YCPValue id() const { return _id; }

    /**
     * Set a new ID.
     **/
    void setId( const YCPValue & newId ) { _id = newId; }

    /**
     * Return this item's label as a YCPString.
     **/
    YCPString label() const { return YCPString( YTreeItem::label() ); }

    /**
     * Set this item's label with a YCPString.
     **/
    void setLabel( const YCPString & newLabel )
	{ YTreeItem::setLabel( newLabel->value() ); }

    /**
     * Return this item's icon name as a YCPString.
     **/
    YCPString iconName() const { return YCPString( YTreeItem::iconName() ); }

    /**
     * Set this item's icon name with a YCPString.
     **/
    void setIconName( const YCPString & newIconName )
	{ YTreeItem::setIconName( newIconName->value() ); }


private:

    YCPValue _id;
};


#endif // YCPTreeItem_h

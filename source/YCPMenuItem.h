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


  File:		YCPMenuItem.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPMenuItem_h
#define YCPMenuItem_h


#include <ycp/YCPValue.h>
#include <ycp/YCPString.h>
#include <yui/YMenuItem.h>


/**
 * Menu item class with YCPValue IDs.
 *
 * Similar to YCPItem (and even more similar to YCPTreeItem, but with YMenuItem
 * as base class. There should be an elegant way to do this with templates, but
 * the different constructor arguments make this difficult.
 **/
class YCPMenuItem: public YMenuItem
{
public:

    /**
     * Constructors
     **/
    YCPMenuItem( const YCPString & 	label,
		 const YCPValue  & 	id )
	: YMenuItem( label->value() )
	, _id( id )
	{}

    YCPMenuItem( const YCPString & 	label,
		 const YCPValue  & 	id,
		 const YCPString & 	iconName )
	: YMenuItem( label->value(), iconName->value() )
	, _id( id )
	{}

    YCPMenuItem( YCPMenuItem *		parent,
		 const YCPString & 	label,
		 const YCPValue  & 	id )
	: YMenuItem( parent, label->value() )
	, _id( id )
	{}

    YCPMenuItem( YCPMenuItem *		parent,
		 const YCPString & 	label,
		 const YCPValue  & 	id,
		 const YCPString & 	iconName )
	: YMenuItem( parent, label->value(), iconName->value() )
	, _id( id )
	{}

    /**
     * Destructor.
     **/
    virtual ~YCPMenuItem()
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
    YCPString label() const { return YCPString( YMenuItem::label() ); }

    /**
     * Set this item's label with a YCPString.
     **/
    void setLabel( const YCPString & newLabel )
	{ YMenuItem::setLabel( newLabel->value() ); }

    /**
     * Return this item's icon name as a YCPString.
     **/
    YCPString iconName() const { return YCPString( YMenuItem::iconName() ); }

    /**
     * Set this item's icon name with a YCPString.
     **/
    void setIconName( const YCPString & newIconName )
	{ YMenuItem::setIconName( newIconName->value() ); }


private:

    YCPValue _id;
};


#endif // YCPMenuItem_h

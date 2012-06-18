/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/


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

  File:		YCPTableItem.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPTableItem_h
#define YCPTableItem_h

#include <ycp/YCPValue.h>
#include <ycp/YCPString.h>
#include <ycp/YCPVoid.h>
#include <yui/YTableItem.h>


/**
 * Item class with YCPValue IDs
 **/
class YCPTableItem: public YTableItem
{
public:

    /**
     * Constructors
     **/
    YCPTableItem( const YCPValue  & id )
	: YTableItem()
	, _id( id )
	{}

    YCPTableItem()
	: YTableItem()
	, _id( YCPVoid() )
	{}

    /**
     * Destructor.
     **/
    virtual ~YCPTableItem()
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
     * Create a new cell and add it (even if both 'label' and
     * 'iconName' are empty).
     **/
    void addCell( const YCPString & label, const YCPString & iconName )
	{ YTableItem::addCell( label->value(), iconName->value() ); }
    
    void addCell( const YCPString & label )
	{ YTableItem::addCell( label->value() ); }


    /**
     * Return the label of cell no. 'index' (counting from 0 on) as a YCPString
     * or an empty YCPString if there is no cell with that index.
     **/
    YCPString label( int index ) const { return YCPString( YTableItem::label( index ) ); }

    /**
     * Return the icon of cell no. 'index' (counting from 0 on) as a YCPString
     * or an empty YCPString if there is no cell with that index.
     **/
    YCPString iconName( int index ) const { return YCPString( YTableItem::iconName( index ) ); }


private:
    
    YCPValue _id;
};


#endif // YCPTableItem_h

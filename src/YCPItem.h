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

  File:		YCPItem.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPItem_h
#define YCPItem_h

#include <ycp/YCPValue.h>
#include <ycp/YCPString.h>
#include "YItem.h"


/**
 * Item class with YCPValue IDs
 **/
class YCPItem: public YItem
{
public:

    /**
     * Constructors
     **/
    YCPItem( const YCPString & 	label,
	     const YCPValue  & 	id,
	     bool  		selected = false )
	: YItem( label->value(), selected )
	, _id( id )
	{}

    YCPItem( const YCPString &	label,
	     const YCPValue  & 	id,
	     const YCPString & 	iconName,
	     bool  		selected = false )
	: YItem( label->value(), iconName->value(), selected )
	, _id( id )
	{}

    /**
     * Destructor.
     **/
    virtual ~YCPItem()
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
    YCPString label() const { return YCPString( YItem::label() ); }

    /**
     * Set this item's label with a YCPString.
     **/
    void setLabel( const YCPString & newLabel )
	{ YItem::setLabel( newLabel->value() ); }

    /**
     * Return this item's icon name as a YCPString.
     **/
    YCPString iconName() const { return YCPString( YItem::iconName() ); }

    /**
     * Set this item's icon name with a YCPString.
     **/
    void setIconName( const YCPString & newIconName )
	{ YItem::setIconName( newIconName->value() ); }


private:
    YCPValue _id;
};


#endif // YCPItem_h

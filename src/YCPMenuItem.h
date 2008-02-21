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

  File:		YCPMenuItem.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPMenuItem_h
#define YCPMenuItem_h


#include <ycp/YCPValue.h>
#include <ycp/YCPString.h>
#include "YMenuItem.h"


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

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

  File:		YCPValueWidgetID.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <ycp/YCPString.h>
#include <ycp/YCPSymbol.h>
#include "YCPValueWidgetID.h"


YCPValueWidgetID::YCPValueWidgetID( const YCPValue & val )
    : _value( val )
{
}


bool
YCPValueWidgetID::isEqual( YWidgetID * otherID ) const
{
    YCPValueWidgetID * otherYCPValueID =
	dynamic_cast<YCPValueWidgetID *> (otherID);

    if ( otherID )
	return _value->equal( otherYCPValueID->value() );

    if ( _value->isString() || _value->isSymbol() )
    {
	YStringWidgetID * otherStringID =
	    dynamic_cast<YStringWidgetID *> (otherID);

	if ( _value->isString() )
	{
	    return _value->asString()->value() == otherStringID->value();
	}
	else // _value->isSymbol()
	{
	    return _value->asSymbol()->symbol() == otherStringID->value();
	}
    }

    return false;
}


string
YCPValueWidgetID::toString() const
{
    return _value->toString();
}


YCPValue
YCPValueWidgetID::value() const
{
    return _value;
}


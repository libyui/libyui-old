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


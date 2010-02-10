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


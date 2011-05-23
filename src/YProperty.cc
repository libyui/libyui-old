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
|								       |
|					  (c) SuSE Linux Products GmbH |
\----------------------------------------------------------------------/

  File:		YProperty.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include "YProperty.h"
#include "YUIException.h"

using std::string;


string
YProperty::typeAsStr( YPropertyType type )
{
    switch ( type )
    {
	case YUnknownPropertyType:	return "<Unknown>";
	case YOtherProperty:		return "<Other>";
	case YStringProperty:		return "String";
	case YBoolProperty:		return "Bool";
	case YIntegerProperty:		return "Integer";

	    // Intentionally omitting default branch
	    // so the compiler catches unhandled enum values
    }

    return "<Undefined property type>";
}


YPropertyValue::~YPropertyValue()
{
}



YPropertySet::YPropertySet()
{
    // NOP
}


void
YPropertySet::check( const string & propertyName ) const
{
    if ( ! contains( propertyName ) )
	YUI_THROW( YUIUnknownPropertyException( propertyName ) );
}


void
YPropertySet::check( const string & propertyName, YPropertyType type ) const
{
    if ( ! contains( propertyName, type ) )
	YUI_THROW( YUIUnknownPropertyException( propertyName ) );

    // YPropertySet::contains( const string &, YPropertyType ) will throw
    // a YUIPropertyTypeMismatchException, if applicable
}


bool
YPropertySet::contains( const string & propertyName ) const throw()
{
    for ( YPropertySet::const_iterator it = _properties.begin();
	  it != _properties.end();
	  ++it )
    {
	if ( it->name() == propertyName )
	    return true;
    }

    return false;
}


bool
YPropertySet::contains( const string & propertyName, YPropertyType type ) const
{
    for ( YPropertySet::const_iterator it = _properties.begin();
	  it != _properties.end();
	  ++it )
    {
	if ( it->name() == propertyName )
	{
	    if ( it->isReadOnly() )
		YUI_THROW( YUISetReadOnlyPropertyException( *it ) );

	    if ( it->type() == type ||
		 it->type() == YOtherProperty )	// "Other" could be anything
		return true;

	    YUI_THROW( YUIPropertyTypeMismatchException( *it, type ) );
	}
    }

    return false;
}


void
YPropertySet::add( const YProperty & prop )
{
    _properties.push_back( prop );
}


void
YPropertySet::add( const YPropertySet & otherSet )
{
    for ( YPropertySet::const_iterator it = otherSet.propertiesBegin();
	  it != otherSet.propertiesEnd();
	  ++it )
    {
	add( *it );
    }
}


YPropertySet::const_iterator
YPropertySet::propertiesBegin() const
{
    return _properties.begin();
}

YPropertySet::const_iterator
YPropertySet::propertiesEnd() const
{
    return _properties.end();
}


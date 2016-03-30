/*
  Copyright (C) 2000-2012 Novell, Inc
  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) version 3.0 of the License. This library
  is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
  License for more details. You should have received a copy of the GNU
  Lesser General Public License along with this library; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301 USA
*/


/*-/

  File:		YProperty.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include "YProperty.h"
#include "YUIException.h"



std::string
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
YPropertySet::check( const std::string & propertyName ) const
{
    if ( ! contains( propertyName ) )
	YUI_THROW( YUIUnknownPropertyException( propertyName ) );
}


void
YPropertySet::check( const std::string & propertyName, YPropertyType type ) const
{
    if ( ! contains( propertyName, type ) )
	YUI_THROW( YUIUnknownPropertyException( propertyName ) );

    // YPropertySet::contains( const std::string &, YPropertyType ) will throw
    // a YUIPropertyTypeMismatchException, if applicable
}


bool
YPropertySet::contains( const std::string & propertyName ) const throw()
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
YPropertySet::contains( const std::string & propertyName, YPropertyType type ) const
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

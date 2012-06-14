/*
  |****************************************************************************
  |
  | Copyright (c) 2000 - 2012 Novell, Inc.
  | All Rights Reserved.
  |
  | This program is free software; you can redistribute it and/or
  | modify it under the terms of version 2 of the GNU General Public License as
  | published by the Free Software Foundation.
  |
  | This program is distributed in the hope that it will be useful,
  | but WITHOUT ANY WARRANTY; without even the implied warranty of
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
  | GNU General Public License for more details.
  |
  | You should have received a copy of the GNU General Public License
  | along with this program; if not, contact Novell, Inc.
  |
  | To contact Novell about this file by physical or electronic mail,
  | you may find current contact information at www.novell.com
  |
  |****************************************************************************
*/


/*-/

  File:		YWidgetID.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <iostream>
#include "YWidgetID.h"



YStringWidgetID::YStringWidgetID( const std::string & val )
    : _value( val )
{
    // NOP
}


YStringWidgetID::~YStringWidgetID()
{
    // NOP
}


bool
YStringWidgetID::isEqual( YWidgetID * otherID ) const
{
    YStringWidgetID * otherStringID =
	dynamic_cast<YStringWidgetID *> (otherID);

    if ( otherStringID )
	return ( this->valueConstRef() == otherStringID->valueConstRef() );
    else
	return false;
}


std::string
YStringWidgetID::toString() const
{
    return _value;
}


std::string
YStringWidgetID::value() const
{
    return _value;
}


const std::string &
YStringWidgetID::valueConstRef() const
{
    return _value;
}


std::ostream & operator<<( std::ostream & stream, const YWidgetID * id )
{
    if ( id )
	stream << id->toString();
    else
	stream << "<NULL YWidgetID>";

    return stream;
}

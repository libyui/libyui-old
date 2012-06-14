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

  File:		YCheckBox.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YCheckBox.h"


struct YCheckBoxPrivate
{
    YCheckBoxPrivate( const std::string & label )
	: label( label )
	, useBoldFont( false )
	{}

    std::string	label;
    bool	useBoldFont;
};


YCheckBox::YCheckBox( YWidget * parent, const std::string & label )
    : YWidget( parent )
    , priv( new YCheckBoxPrivate( label ) )
{
    YUI_CHECK_NEW( priv );
}


YCheckBox::~YCheckBox()
{
    // NOP
}


void YCheckBox::setLabel( const std::string & newLabel )
{
    priv->label = newLabel;
}


std::string YCheckBox::label() const
{
    return priv->label;
}


bool YCheckBox::useBoldFont() const
{
    return priv->useBoldFont;
}


void YCheckBox::setUseBoldFont( bool bold )
{
    priv->useBoldFont = bold;
}


const YPropertySet &
YCheckBox::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property boolean	  Value 	the on/off state; nil for "don't care" (tristate)
	 * @property std::string  Label		the text on the CheckBox
	 */

	propSet.add( YProperty( YUIProperty_Value,	YOtherProperty	) );
	propSet.add( YProperty( YUIProperty_Label,	YStringProperty	) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YCheckBox::setProperty( const std::string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if      ( propertyName == YUIProperty_Value )	return false; // need special processing
    else if ( propertyName == YUIProperty_Label )	setLabel( val.stringVal() );
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YCheckBox::getProperty( const std::string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if      ( propertyName == YUIProperty_Value )	return YPropertyValue( YOtherProperty );
    else if ( propertyName == YUIProperty_Label )	return YPropertyValue( label() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}

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

  File:		YFrame.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YFrame.h"
#include "YShortcut.h"

struct YFramePrivate
{
    YFramePrivate( const std::string & frameLabel )
	: label( frameLabel )
	{}

    std::string label;
};




YFrame::YFrame( YWidget * parent, const std::string & label )
    : YSingleChildContainerWidget( parent )
    , priv( new YFramePrivate( YShortcut::cleanShortcutString( label ) ) )
{
    YUI_CHECK_NEW( priv );
}


YFrame::~YFrame()
{
    // NOP
}


void YFrame::setLabel( const std::string & newLabel )
{
    priv->label = YShortcut::cleanShortcutString( newLabel );
}


std::string YFrame::label() const
{
    return priv->label;
}


const YPropertySet &
YFrame::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property std::string	Label	the text on the frame
	 */

	propSet.add( YProperty( YUIProperty_Label,	YStringProperty	) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YFrame::setProperty( const std::string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if      ( propertyName == YUIProperty_Label )	setLabel( val.stringVal() );
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YFrame::getProperty( const std::string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if      ( propertyName == YUIProperty_Label )	return YPropertyValue( label() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}

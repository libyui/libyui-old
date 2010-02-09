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
    YFramePrivate( const string & frameLabel )
	: label( frameLabel )
	{}

    string label;
};




YFrame::YFrame( YWidget * parent, const string & label )
    : YSingleChildContainerWidget( parent )
    , priv( new YFramePrivate( YShortcut::cleanShortcutString( label ) ) )
{
    YUI_CHECK_NEW( priv );
}


YFrame::~YFrame()
{
    // NOP
}


void YFrame::setLabel( const string & newLabel )
{
    priv->label = YShortcut::cleanShortcutString( newLabel );
}


string YFrame::label() const
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
	 * @property string  Label	the text on the frame
	 */
	
	propSet.add( YProperty( YUIProperty_Label,	YStringProperty	) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YFrame::setProperty( const string & propertyName, const YPropertyValue & val )
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
YFrame::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if      ( propertyName == YUIProperty_Label )	return YPropertyValue( label() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}


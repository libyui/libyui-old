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

  File:		YRichText.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YRichText.h"


struct YRichTextPrivate
{
    /**
     * Constructor.
     **/
    YRichTextPrivate( const string & text, bool plainTextMode )
	: text( text )
	, plainTextMode( plainTextMode )
	, autoScrollDown ( false )
	, shrinkable( false )
	{}
    
    string	text;
    bool	plainTextMode;
    bool	autoScrollDown;
    bool	shrinkable;
};




YRichText::YRichText( YWidget * parent, const string & text, bool plainTextMode )
    : YWidget( parent )
    , priv( new YRichTextPrivate( text, plainTextMode ) )
{
    YUI_CHECK_NEW( priv );
    
    setDefaultStretchable( YD_HORIZ, true );
    setDefaultStretchable( YD_VERT,  true );
}


YRichText::~YRichText()
{
    // NOP
}


void YRichText::setValue( const string & newValue )
{
    priv->text = newValue;
}


string YRichText::value() const
{
    return priv->text;
}


bool YRichText::plainTextMode() const
{
    return priv->plainTextMode;
}


void YRichText::setPlainTextMode( bool plainTextMode )
{
    priv->plainTextMode = plainTextMode;
}


bool YRichText::autoScrollDown() const
{
    return priv->autoScrollDown;
}


void YRichText::setAutoScrollDown( bool autoScrollDown )
{
    priv->autoScrollDown = autoScrollDown;
}


bool YRichText::shrinkable() const
{
    return priv->shrinkable;
}


void YRichText::setShrinkable( bool shrinkable )
{
    priv->shrinkable = shrinkable;
}


const YPropertySet &
YRichText::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property string  Value	the text content
	 * @property string  Text	the text content
	 */
	propSet.add( YProperty( YUIProperty_Value,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_Text,		YStringProperty	 ) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YRichText::setProperty( const string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if	    ( propertyName == YUIProperty_Value		)	setValue( val.stringVal() );
    if	    ( propertyName == YUIProperty_Text		)	setValue( val.stringVal() );
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YRichText::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if	    ( propertyName == YUIProperty_Value		)	return YPropertyValue( value() );
    else if ( propertyName == YUIProperty_Text		)	return YPropertyValue( value() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}


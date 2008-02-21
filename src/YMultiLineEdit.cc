/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|      	                        core system                            |
|                                                        (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:		YMultiLineEdit.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YMultiLineEdit.h"


#define DEFAULT_VISIBLE_LINES	3


struct YMultiLineEditPrivate
{
    YMultiLineEditPrivate( const string & label )
	: label( label )
	, inputMaxLength( -1 )
	, defaultVisibleLines( DEFAULT_VISIBLE_LINES )
	{}
    
    string	label;
    int		inputMaxLength;
    int		defaultVisibleLines;
};




YMultiLineEdit::YMultiLineEdit( YWidget * parent, const string & label )
    : YWidget( parent )
    , priv( new YMultiLineEditPrivate( label ) )
{
    YUI_CHECK_NEW( priv );
    
    setDefaultStretchable( YD_HORIZ, true );
    setDefaultStretchable( YD_VERT,  true );
}


YMultiLineEdit::~YMultiLineEdit()
{
    // NOP
}


string YMultiLineEdit::label() const
{
    return priv->label;
}


void YMultiLineEdit::setLabel( const string & label )
{
    priv->label = label;
}


int YMultiLineEdit::inputMaxLength() const
{
    return priv->inputMaxLength;
}


void YMultiLineEdit::setInputMaxLength( int len )
{
    priv->inputMaxLength = len;
}


int YMultiLineEdit::defaultVisibleLines() const
{
    return priv->defaultVisibleLines;
}


void YMultiLineEdit::setDefaultVisibleLines( int newVisibleLines )
{
    priv->defaultVisibleLines = newVisibleLines;
}


const YPropertySet &
YMultiLineEdit::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property string  Value		the MultiLineEdit text contents (with newlines)
	 * @property string  Label		caption above the MultiLineEdit
	 * @property integer InputMaxLength	maximum number of input characters
	 */
	propSet.add( YProperty( YUIProperty_Value,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_Label,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_InputMaxLength,	YIntegerProperty ) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YMultiLineEdit::setProperty( const string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if	    ( propertyName == YUIProperty_Value		)	setValue( val.stringVal() );
    else if ( propertyName == YUIProperty_Label		)	setLabel( val.stringVal() );
    else if ( propertyName == YUIProperty_InputMaxLength )	setInputMaxLength( val.integerVal() );
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YMultiLineEdit::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if	    ( propertyName == YUIProperty_Value		)	return YPropertyValue( value() );
    else if ( propertyName == YUIProperty_Label		)	return YPropertyValue( label() );
    else if ( propertyName == YUIProperty_InputMaxLength )	return YPropertyValue( inputMaxLength() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}


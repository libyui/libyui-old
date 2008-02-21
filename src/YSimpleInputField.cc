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

  File:		YSimpleInputField.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YSimpleInputField.h"


struct YSimpleInputFieldPrivate
{
    YSimpleInputFieldPrivate( const string & label )
	: label( label )
	{}

    string label;
};




YSimpleInputField::YSimpleInputField( YWidget * parent, const string & label )
    : YWidget( parent )
    , priv( new YSimpleInputFieldPrivate( label ) )
{
    YUI_CHECK_NEW( priv );

    setDefaultStretchable( YD_HORIZ, false );
    setDefaultStretchable( YD_VERT,  false );
}


YSimpleInputField::~YSimpleInputField()
{
    // NOP
}


string YSimpleInputField::label() const
{
    return priv->label;
}


void YSimpleInputField::setLabel( const string & label )
{
    priv->label = label;
}



const YPropertySet &
YSimpleInputField::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property string  Value	the text the user entered
	 * @property string  Label	caption above the input field
	 */
	propSet.add( YProperty( YUIProperty_Value,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_Label,		YStringProperty	 ) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YSimpleInputField::setProperty( const string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if	    ( propertyName == YUIProperty_Value		)	setValue( val.stringVal() );
    else if ( propertyName == YUIProperty_Label		)	setLabel( val.stringVal() );
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YSimpleInputField::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if	    ( propertyName == YUIProperty_Value		)	return YPropertyValue( value() );
    else if ( propertyName == YUIProperty_Label		)	return YPropertyValue( label() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}


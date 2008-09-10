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

  File:		YInputField.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YMacroRecorder.h"
#include "YInputField.h"

using std::string;


struct YInputFieldPrivate
{
    YInputFieldPrivate( string label, bool passwordMode )
	: label( label )
	, passwordMode( passwordMode )
	, shrinkable( false )
	, inputMaxLength( -1 )
	{}
    
    string 	label;
    bool	passwordMode;
    bool	shrinkable;
    string 	validChars;
    int		inputMaxLength;
};



YInputField::YInputField( YWidget * parent, const string & label, bool passwordMode )
    : YWidget( parent )
    , priv( new YInputFieldPrivate( label, passwordMode ) )
{
    YUI_CHECK_NEW( priv );

    // setDefaultStretchable( YD_HORIZ, true  );
    setDefaultStretchable( YD_VERT,  false );
}


YInputField::~YInputField()
{
    // NOP
}


string YInputField::label() const
{
    return priv->label;
}


void YInputField::setLabel( const string & label )
{
    priv->label = label;
}


bool YInputField::passwordMode() const
{
    return priv->passwordMode;
}


bool YInputField::shrinkable() const
{
    return priv->shrinkable;
}


void YInputField::setShrinkable( bool shrinkable )
{
    priv->shrinkable = shrinkable;
    // setDefaultStretchable( YD_HORIZ, ! shrinkable );
}


string YInputField::validChars()
{
    return priv->validChars;
}


void YInputField::setValidChars( const string & newValidChars )
{
    priv->validChars= newValidChars;
}


int YInputField::inputMaxLength() const
{
    return priv->inputMaxLength;
}


void YInputField::setInputMaxLength( int len )
{
    priv->inputMaxLength = len;
}


const YPropertySet &
YInputField::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property string  Value		the input field's contents (the user input)
	 * @property string  Label		caption above the input field
	 * @property string  ValidChars		set of valid input characters
	 * @property integer InputMaxLength	maximum number of input characters
	 */
	propSet.add( YProperty( YUIProperty_Value,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_Label,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_ValidChars,		YStringProperty	 ) );
	propSet.add( YProperty( YUIProperty_InputMaxLength,	YIntegerProperty ) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YInputField::setProperty( const string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if	    ( propertyName == YUIProperty_Value		)	setValue( val.stringVal() );
    else if ( propertyName == YUIProperty_Label		)	setLabel( val.stringVal() );
    else if ( propertyName == YUIProperty_ValidChars	)	setValidChars( val.stringVal() );
    else if ( propertyName == YUIProperty_InputMaxLength )	setInputMaxLength( val.integerVal() );
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YInputField::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if	    ( propertyName == YUIProperty_Value		)	return YPropertyValue( value() );
    else if ( propertyName == YUIProperty_Label		)	return YPropertyValue( label() );
    else if ( propertyName == YUIProperty_ValidChars	)	return YPropertyValue( validChars() );
    else if ( propertyName == YUIProperty_InputMaxLength )	return YPropertyValue( inputMaxLength() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}


void
YInputField::saveUserInput( YMacroRecorder *macroRecorder )
{
    if ( ! passwordMode() ) // Don't record passwords in the macro file
    {
	macroRecorder->recordWidgetProperty( this, YUIProperty_Value );
    }
}


const char *
YInputField::widgetClass() const
{
    if ( priv->passwordMode )	return "YPasswordField";
    else			return "YInputField";
}

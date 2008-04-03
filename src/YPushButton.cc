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

  File:		YPushButton.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUI.h"
#include "YApplication.h"
#include "YDialog.h"
#include "YUISymbols.h"
#include "YPushButton.h"


using std::string;


struct YPushButtonPrivate
{
    YPushButtonPrivate( const string & label )
	: label( label )
	, isDefaultButton( false )
	, setDefaultButtonRecursive( false )
	, isHelpButton( false )
	{}
    
    string	label;
    bool	isDefaultButton;
    bool	setDefaultButtonRecursive;
    bool	isHelpButton;
};


YPushButton::YPushButton( YWidget * parent, const string & label )
    : YWidget( parent )
    , priv( new YPushButtonPrivate( label ) )
{
    int fkey = YUI::app()->defaultFunctionKey( label );

    if ( fkey > 0 && ! hasFunctionKey() )
	setFunctionKey( fkey );
}


YPushButton::~YPushButton()
{
    YDialog * dialog = findDialog();
    
    if ( dialog && dialog->defaultButton() == this )
    {
	dialog->setDefaultButton( 0 );
    }
}


void YPushButton::setLabel( const string & label )
{
    priv->label = label;
}


string YPushButton::label() const
{
    return priv->label;
}


bool YPushButton::isDefaultButton() const
{
    return priv->isDefaultButton;
}


void YPushButton::setDefaultButton( bool isDefaultButton )
{
    priv->isDefaultButton = isDefaultButton;

    if ( ! priv->setDefaultButtonRecursive )
    {
	// Prevent endless recursion if dialog->setDefaultButton()
	// calls this function again
	
	priv->setDefaultButtonRecursive = true;

	YDialog * dialog = findDialog();
    
	if ( dialog )
	{
	    if ( isDefaultButton )
		dialog->setDefaultButton( this );
	    else
	    {
		if ( dialog->defaultButton() == this )
		    dialog->setDefaultButton( 0 );
	    }
	}
	
	priv->setDefaultButtonRecursive = false;
    }
}


bool YPushButton::isHelpButton() const
{
    return priv->isHelpButton;
}


void YPushButton::setHelpButton( bool helpButton )
{
    priv->isHelpButton = helpButton;
}



const YPropertySet &
YPushButton::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property string Label	text on the button
	 */
	propSet.add( YProperty( YUIProperty_Label, YStringProperty ) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YPushButton::setProperty( const string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if ( propertyName == YUIProperty_Label )	setLabel( val.stringVal() );
    else
    {
	YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special handling necessary
}


YPropertyValue
YPushButton::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if ( propertyName == YUIProperty_Label )	return YPropertyValue( label() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}



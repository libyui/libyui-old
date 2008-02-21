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

  File:		YRadioButtonGroup.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YRadioButton.h"
#include "YRadioButtonGroup.h"


struct YRadioButtonGroupPrivate
{
    YRadioButtonGroupPrivate()
	{}


    YRadioButtonList buttonList;
};




YRadioButtonGroup::YRadioButtonGroup( YWidget * parent )
    : YSingleChildContainerWidget( parent )
    , priv( new YRadioButtonGroupPrivate() )
{
    YUI_CHECK_NEW( priv );
}


YRadioButtonGroup::~YRadioButtonGroup()
{
}


YRadioButtonListConstIterator
YRadioButtonGroup::radioButtonsBegin() const
{
    return priv->buttonList.begin();
}


YRadioButtonListConstIterator
YRadioButtonGroup::radioButtonsEnd() const
{
    return priv->buttonList.end();
}


int
YRadioButtonGroup::radioButtonsCount() const
{
    return priv->buttonList.size();
}


void
YRadioButtonGroup::addRadioButton( YRadioButton * button )
{
    priv->buttonList.push_back( button );
}


void
YRadioButtonGroup::removeRadioButton( YRadioButton * button )
{
    priv->buttonList.remove( button );
}


void
YRadioButtonGroup::uncheckOtherButtons( YRadioButton * selectedRadioButton )
{
    for ( YRadioButtonListConstIterator it = radioButtonsBegin();
	  it != radioButtonsEnd();
	  ++it )
    {
	if ( *it != selectedRadioButton )
	    (*it)->setValue( false );
    }
}


YRadioButton *
YRadioButtonGroup::currentButton() const
{
    for ( YRadioButtonListConstIterator it = radioButtonsBegin();
	  it != radioButtonsEnd();
	  ++it )
    {
	if ( (*it)->value() )
	    return *it;
    }

    return 0;
}


const YPropertySet &
YRadioButtonGroup::propertySet()
{
    static YPropertySet propSet;
    
    if ( propSet.isEmpty() )
    {
	/*
	 * @property any CurrentButton	widget ID of the currently selected RadioButton of this group
	 * @property any Value		Alias for CurrentButton
	 */
	propSet.add( YProperty( YUIProperty_Value,		YOtherProperty ) );
	propSet.add( YProperty( YUIProperty_CurrentButton,	YOtherProperty ) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YRadioButtonGroup::setProperty( const string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if ( propertyName == YUIProperty_CurrentButton ||
	 propertyName == YUIProperty_Value )		return false; // Needs special handling
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YRadioButtonGroup::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if ( propertyName == YUIProperty_CurrentButton ||
	 propertyName == YUIProperty_Value )		return YPropertyValue( YOtherProperty );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}



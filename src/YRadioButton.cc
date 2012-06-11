/*************************************************************************************************************

 Copyright (C) 2000 - 2010 Novell, Inc.   All Rights Reserved.

 This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 Public License as published by the Free Software Foundation; either version 2 of the License, or (at your
 option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License along with this program; if not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*************************************************************************************************************/



 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////   __/\\\\\\_____________/\\\__________/\\\________/\\\___/\\\________/\\\___/\\\\\\\\\\\_           ////
 ////    _\////\\\____________\/\\\_________\///\\\____/\\\/___\/\\\_______\/\\\__\/////\\\///__          ////
 ////     ____\/\\\______/\\\__\/\\\___________\///\\\/\\\/_____\/\\\_______\/\\\______\/\\\_____         ////
 ////      ____\/\\\_____\///___\/\\\_____________\///\\\/_______\/\\\_______\/\\\______\/\\\_____        ////
 ////       ____\/\\\______/\\\__\/\\\\\\\\\_________\/\\\________\/\\\_______\/\\\______\/\\\_____       ////
 ////        ____\/\\\_____\/\\\__\/\\\////\\\________\/\\\________\/\\\_______\/\\\______\/\\\_____      ////
 ////         ____\/\\\_____\/\\\__\/\\\__\/\\\________\/\\\________\//\\\______/\\\_______\/\\\_____     ////
 ////          __/\\\\\\\\\__\/\\\__\/\\\\\\\\\_________\/\\\_________\///\\\\\\\\\/_____/\\\\\\\\\\\_    ////
 ////           _\/////////___\///___\/////////__________\///____________\/////////______\///////////__   ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                 widget abstraction library providing Qt, GTK and ncurses frontends                  ////
 ////                                                                                                     ////
 ////                                   3 UIs for the price of one code                                   ////
 ////                                                                                                     ////
 ////                                       ***  core library  ***                                        ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                              (C) SUSE Linux GmbH    ////
 ////                                                                                                     ////
 ////                                                              libYUI-AsciiArt (C) 2012 Björn Esser   ////
 ////                                                                                                     ////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-/

  File:		YRadioButton.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <string>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUISymbols.h"
#include "YUIException.h"
#include "YMacroRecorder.h"
#include "YRadioButtonGroup.h"
#include "YRadioButton.h"



struct YRadioButtonPrivate
{
    /**
     * Constructor
     **/
    YRadioButtonPrivate( const std::string & label )
	: label( label )
	, radioButtonGroup( 0 )
	, useBoldFont( false )
	{}

    //
    // Data members
    //

    std::string		label;
    YRadioButtonGroup * radioButtonGroup;
    bool		useBoldFont;
};


YRadioButton::YRadioButton( YWidget * 		parent,
			    const std::string &	label )
    : YWidget( parent )
    , priv( new YRadioButtonPrivate( label ) )
{
    YUI_CHECK_NEW( priv );

    // Intentionally not calling
    //     buttonGroup()->addRadioButton( this );
    // here because virtual functions can't be used yet (while the constructor
    // isn't finished yet), and the RadioButtonGroup for sure would try to call
    // YRadioButton::value() which is (pure) virtual, thus not available yet.
    //
    // The caller has to take care of this.
}


YRadioButton::~YRadioButton()
{
    if ( priv->radioButtonGroup )
    {
	if ( ! priv->radioButtonGroup->beingDestroyed() )
	    priv->radioButtonGroup->removeRadioButton( this );
    }
}


void YRadioButton::setLabel( const std::string & newLabel )
{
    priv->label = newLabel;
}


std::string YRadioButton::label() const
{
    return priv->label;
}


bool YRadioButton::useBoldFont() const
{
    return priv->useBoldFont;
}


void YRadioButton::setUseBoldFont( bool bold )
{
    priv->useBoldFont = bold;
}


const YPropertySet &
YRadioButton::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property boolean		Value	the on/off state of the RadioButton
	 * @property std::string	Label	the text on the RadioButton
	 */

	propSet.add( YProperty( YUIProperty_Value,	YBoolProperty	) );
	propSet.add( YProperty( YUIProperty_Label,	YStringProperty	) );
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


bool
YRadioButton::setProperty( const std::string & propertyName, const YPropertyValue & val )
{
    propertySet().check( propertyName, val.type() ); // throws exceptions if not found or type mismatch

    if      ( propertyName == YUIProperty_Value )	setValue( val.boolVal() );
    else if ( propertyName == YUIProperty_Label )	setLabel( val.stringVal() );
    else
    {
	return YWidget::setProperty( propertyName, val );
    }

    return true; // success -- no special processing necessary
}


YPropertyValue
YRadioButton::getProperty( const std::string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if      ( propertyName == YUIProperty_Value )	return YPropertyValue( value() );
    else if ( propertyName == YUIProperty_Label )	return YPropertyValue( label() );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}


YRadioButtonGroup *
YRadioButton::buttonGroup()
{
    if ( ! priv->radioButtonGroup )
    {
	priv->radioButtonGroup = findRadioButtonGroup();
    }

    return priv->radioButtonGroup;
}


YRadioButtonGroup *
YRadioButton::findRadioButtonGroup() const
{
    YWidget * widget = parent();

    while ( widget )
    {
	YRadioButtonGroup * radioButtonGroup = dynamic_cast<YRadioButtonGroup *> (widget);

	if ( radioButtonGroup )
	    return radioButtonGroup;
	else
	    widget = widget->parent();
    }

    return 0;
}


void
YRadioButton::saveUserInput( YMacroRecorder *macroRecorder )
{
    if ( value() )
    {
	// Only record if this radio button is on. By definition one radio
	// button of the radio box _must_ be on if the user did anything, so we
	// don't record a lot of redundant "ChangeWidget( ..., `Value, false )"
	// calls.

	macroRecorder->recordWidgetProperty( this, YUIProperty_Value );
    }
}

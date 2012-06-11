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
YRadioButtonGroup::setProperty( const std::string & propertyName, const YPropertyValue & val )
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
YRadioButtonGroup::getProperty( const std::string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if ( propertyName == YUIProperty_CurrentButton ||
	 propertyName == YUIProperty_Value )		return YPropertyValue( YOtherProperty );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}

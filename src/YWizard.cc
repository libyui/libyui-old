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

  File:		YWizard.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YWizard.h"
#include "YPushButton.h"


struct YWizardPrivate
{
    YWizardPrivate( YWizardMode wizardMode )
	: wizardMode( wizardMode )
	, nextButtonIsProtected( false )
	{}

    YWizardMode	wizardMode;
    bool	nextButtonIsProtected;
};




YWizard::YWizard( YWidget *		parent,
		  const string & 	backButtonLabel,
		  const string & 	abortButtonLabel,
		  const string & 	nextButtonLabel,
		  YWizardMode 		wizardMode )
    : YWidget( parent )
    , priv( new YWizardPrivate( wizardMode ) )
{
    YUI_CHECK_NEW( priv );

    // On the YWidget level, a Wizard has a content area and a couple of
    // buttons as children, so simply subclassing from YSimpleChildManager
    // won't do; a children manager that can handle more children is needed.
    setChildrenManager( new YWidgetChildrenManager( this ) );

    setDefaultStretchable( YD_HORIZ, true );
    setDefaultStretchable( YD_VERT,  true );
}


YWizard::~YWizard()
{
    // NOP
}


YWizardMode
YWizard::wizardMode() const
{
    return priv->wizardMode;
}

bool
YWizard::nextButtonIsProtected() const
{
    return priv->nextButtonIsProtected;
}


void
YWizard::protectNextButton( bool protect )
{
    priv->nextButtonIsProtected = protect;
}


void
YWizard::setButtonLabel( YPushButton * button, const string & label )
{
    // FIXME: Throw exception? ( YUI_CHECK_PTR() )

    if ( button )
	button->setLabel( label );
    else
	yuiError() << "NULL button" << endl;
}


void
YWizard::ping()
{
    yuiDebug() << "YWizard is active" << endl;
}


const YPropertySet &
YWizard::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property string CurrentItem	the currently selected tree item (read only)
	 */
	propSet.add( YProperty( YUIProperty_CurrentItem, YStringProperty, true ) );	// read-only
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


YPropertyValue
YWizard::getProperty( const string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if ( propertyName == YUIProperty_CurrentItem )	return YPropertyValue( YOtherProperty );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}

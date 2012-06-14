/*
  |****************************************************************************
  |
  | Copyright (c) 2000 - 2012 Novell, Inc.
  | All Rights Reserved.
  |
  | This program is free software; you can redistribute it and/or
  | modify it under the terms of version 2 of the GNU General Public License as
  | published by the Free Software Foundation.
  |
  | This program is distributed in the hope that it will be useful,
  | but WITHOUT ANY WARRANTY; without even the implied warranty of
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
  | GNU General Public License for more details.
  |
  | You should have received a copy of the GNU General Public License
  | along with this program; if not, contact Novell, Inc.
  |
  | To contact Novell about this file by physical or electronic mail,
  | you may find current contact information at www.novell.com
  |
  |****************************************************************************
*/


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
		  const std::string & 	backButtonLabel,
		  const std::string & 	abortButtonLabel,
		  const std::string & 	nextButtonLabel,
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
YWizard::setButtonLabel( YPushButton * button, const std::string & label )
{
    // FIXME: Throw exception? ( YUI_CHECK_PTR() )

    if ( button )
	button->setLabel( label );
    else
	yuiError() << "NULL button" << std::endl;
}


void
YWizard::ping()
{
    yuiDebug() << "YWizard is active" << std::endl;
}


const YPropertySet &
YWizard::propertySet()
{
    static YPropertySet propSet;

    if ( propSet.isEmpty() )
    {
	/*
	 * @property std::string	CurrentItem	the currently selected tree item (read only)
	 */
	propSet.add( YProperty( YUIProperty_CurrentItem, YStringProperty, true ) );	// read-only
	propSet.add( YWidget::propertySet() );
    }

    return propSet;
}


YPropertyValue
YWizard::getProperty( const std::string & propertyName )
{
    propertySet().check( propertyName ); // throws exceptions if not found

    if ( propertyName == YUIProperty_CurrentItem )	return YPropertyValue( YOtherProperty );
    else
    {
	return YWidget::getProperty( propertyName );
    }
}

/****************************************************************************

Copyright (c) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, contact Novell, Inc.

To contact Novell about this file by physical or electronic mail,
you may find current contact information at www.novell.com

****************************************************************************

  File:		YCPErrorDialog.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <ycp/YCPSymbol.h>

#define YUILogComponent "ui"
#include <yui/YUILog.h>

#include "YCPErrorDialog.h"
#include <yui/YUI.h>
#include <yui/YApplication.h>

#include <yui/YWidgetFactory.h>
#include "YCPValueWidgetID.h"
#include <yui/YDialog.h>
#include <yui/YLabel.h>
#include <yui/YPushButton.h>
#include <yui/YButtonBox.h>
#include <yui/YLayoutBox.h>

using std::string;


void YCPErrorDialog::exceptionDialog( const string & headingText,
				      const YUIException & exception ) throw()
{
    try
    {
	YWidgetFactory * fac = YUI::widgetFactory();
	YDialog * dialog = fac->createPopupDialog( YDialogWarnColor );
	YLayoutBox * vbox = fac->createVBox( dialog );
	fac->createHeading( vbox, headingText );
	fac->createLabel( vbox, exception.asString() );
	fac->createLabel( vbox, "Check the log file!" );
	YButtonBox  * buttonBox   = fac->createButtonBox( vbox );
	YPushButton * closeButton = fac->createPushButton( buttonBox, "&Close" );
	closeButton->setRole( YOKButton );
	dialog->open();

	dialog->waitForEvent();
	dialog->destroy();
    }
    catch ( YUIException & ex )
    {
	YUI_CAUGHT( exception );

	// No other action. If opening this error dialog fails, the user will
	// just have to live with the error being reported in the log file only.
    }
    catch ( ... )
    {
	yuiWarning() << "Caught unknown exception" << std::endl;
    }
}


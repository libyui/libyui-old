/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/


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

  File:		YCPErrorDialog.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include <ycp/YCPSymbol.h>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YCPErrorDialog.h"
#include "YUI.h"
#include "YApplication.h"

#include "YWidgetFactory.h"
#include "YCPValueWidgetID.h"
#include "YDialog.h"
#include "YLabel.h"
#include "YPushButton.h"
#include "YButtonBox.h"
#include "YLayoutBox.h"

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
	yuiWarning() << "Caught unknown exception" << endl;
    }
}


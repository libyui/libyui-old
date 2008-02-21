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
	YLayoutBox * buttonBox = fac->createHBox( vbox );
	fac->createHStretch( buttonBox );
	YPushButton * closeButton = fac->createPushButton( buttonBox, "&Close" );
	closeButton->setId( new YCPValueWidgetID( YCPSymbol( "cancel" ) ) );
	dialog->open();

	YEvent * event = dialog->waitForEvent();
	delete event;
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


/*
  Copyright (c) [2019] SUSE LLC
  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) version 3.0 of the License. This library
  is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
  License for more details. You should have received a copy of the GNU
  Lesser General Public License along with this library; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301 USA
*/


// Example for ItemSelector with custom status values.
//
// Compile with:
//
//     g++ -I/usr/include/yui -lyui ItemSelector3-custom-states.cc -o ItemSelector3-custom-states
//
// For a very simple ItemSelector example, see ItemSelector2-minimalistic.cc.

#define YUILogComponent "example"
#include "YUILog.h"

#include "YUI.h"
#include "YWidgetFactory.h"
#include "YDialog.h"
#include "YAlignment.h"
#include "YButtonBox.h"
#include "YCheckBox.h"
#include "YFrame.h"
#include "YIntField.h"
#include "YItemCustomStatus.h"
#include "YItemSelector.h"
#include "YLabel.h"
#include "YLayoutBox.h"
#include "YPushButton.h"
#include "YRadioButton.h"
#include "YRadioButtonGroup.h"
#include "YEvent.h"


using std::string;

enum CustomStatus
{
    MOD_DONT_INSTALL = 0,
    MOD_INSTALL,
    MOD_AUTOINSTALL
};

bool notify		= true;
int  visibleItems	= 6;

YDescribedItem * modKde;
YDescribedItem * modXfce;
YDescribedItem * modX11;
YDescribedItem * modLibre;
YDescribedItem * modServer;
YDescribedItem * modSdk;


// Widgets

YDialog		* dialog	= 0;
YItemSelector	* selector	= 0;
YLabel		* valueField	= 0;
YPushButton	* valueButton	= 0;
YLabel		* resultField	= 0;
YPushButton	* resultButton	= 0;
YPushButton	* focusButton	= 0;
YPushButton	* closeButton	= 0;


void createWidgets();
void handleEvents();
void handleDependencies( YItem * item );
void autoSelect( YItem * item );
void updateValueField();
void updateResultField();
void askConfigValues();


int main( int argc, char **argv )
{
    YUILog::setLogFileName( "/tmp/libyui-examples.log" );
    YUILog::enableDebugLogging();

    createWidgets();
    handleEvents();

    dialog->destroy();
}


/**
 * Create the widget tree for the main dialog
 **/
void createWidgets()
{
    yuiMilestone() << endl;

    YWidgetFactory * fac = YUI::widgetFactory();

    dialog		= fac->createPopupDialog();
    YAlignment * mbox	= fac->createMarginBox( dialog, 2, 2, 0.4, 0.4 );
    YLayoutBox * vbox	= fac->createVBox( mbox );

    fac->createHeading( vbox, "Add-on Software" );
    fac->createVSpacing( vbox, 0.2 );

    YItemCustomStatusVector states;
    //                                   iconName              textIndicator  nextStatus
    states.push_back( YItemCustomStatus( "checkbox-off",            "[  ]", MOD_INSTALL      ) );
    states.push_back( YItemCustomStatus( "checkbox-on",             "[ +]", MOD_DONT_INSTALL ) );
    states.push_back( YItemCustomStatus( "checkbox-auto-selected",  "[a+]", MOD_DONT_INSTALL ) );

    selector = fac->createCustomStatusItemSelector( vbox, states );
    YUI_CHECK_PTR( selector );

    modKde    = new YDescribedItem( "KDE Plasma",       "Full-fledged desktop"                  );
    modXfce   = new YDescribedItem( "Xfce",             "Lightweight desktop"                   );
    modX11    = new YDescribedItem( "X Window System",  "X11, simple window manager, xterm"     );
    modLibre  = new YDescribedItem( "LibreOffice",      "Office suite"                          );
    modServer = new YDescribedItem( "Server Tools",	"Web server, database, file server"     );
    modSdk    = new YDescribedItem( "SDK",              "Development tools"                     );

    YItemCollection items;
    items.push_back( modKde    );
    items.push_back( modXfce   );
    items.push_back( modX11    );
    items.push_back( modLibre  );
    items.push_back( modServer );
    items.push_back( modSdk    );
    selector->addItems( items ); // This is more efficient than repeatedly calling selector->addItem()

    selector->setVisibleItems( visibleItems );

    if ( notify )
	selector->setNotify();

    fac->createVSpacing( vbox, 0.4 );

    YLayoutBox * hbox1	= fac->createHBox( vbox );
    valueField		= fac->createOutputField( hbox1, "<unknown>" );
    valueField->setStretchable( YD_HORIZ, true ); // allow stretching over entire dialog width

    valueButton		= fac->createPushButton( hbox1, "&Value" );

    fac->createVSpacing( vbox, 0.3 );

    YLayoutBox * hbox2	= fac->createHBox( vbox );
    resultField		= fac->createOutputField( hbox2, "<selected items>\n\n\n\n\n" );
    resultField->setStretchable( YD_HORIZ, true ); // allow stretching over entire dialog width

    resultButton	= fac->createPushButton( hbox2, "&Selected\nItems" );

    fac->createVSpacing( vbox, 0.3 );

    YAlignment	* alignment1	= fac->createLeft( vbox );
    YLayoutBox	* buttonBox	= fac->createHBox( alignment1 );
    focusButton			= fac->createPushButton( buttonBox, "Set &Focus" );

    YAlignment * rightAlignment = fac->createRight( vbox );
    closeButton			= fac->createPushButton( rightAlignment, "&Close" );
}


/**
 * Event loop and event handling for the main dialog
 **/
void handleEvents()
{
    yuiMilestone() << endl;

    while ( true )
    {
	YEvent * event = dialog->waitForEvent();

	if ( event )
	{
	    if ( event->widget()    == closeButton ||
		 event->eventType() == YEvent::CancelEvent ) // window manager "close window" button
	    {
		break; // leave event loop
	    }

	    if ( event->widget() == focusButton )
		selector->setKeyboardFocus();

	    if ( event->widget() == resultButton )
		updateResultField();

	    if ( event->widget() == valueButton )
		updateValueField();

	    if ( event->widget() == selector )
	    {
                // No YWidgetEvents from the ItemSelector in this mode (see below)
	    }

            if ( event->eventType() == YEvent::MenuEvent )
            {
                // In this mode, the ItemSelector does not send YWidgetEvents,
                // but YMenuEvents since (unlike YWidgetEvents) they can
                // return the item that was changed.
                //
                // Still, this requires setNotify() (the notify option to be set).

                YMenuEvent * menuEvent = dynamic_cast<YMenuEvent *>( event );

                if ( menuEvent )
                    handleDependencies( menuEvent->item() );

		updateResultField();
		updateValueField();
            }
	}
    }
}


void handleDependencies( YItem * item )
{
    yuiMilestone() << endl;

    if ( ! item )
    {
        yuiWarning() << "No item in YMenuEvent??" << endl;
        return;
    }

    bool needX11   = false;
    bool needLibre = false;

    if ( modKde->selected() )
    {
        needX11   = true;
        needLibre = true;
    }

    if ( modXfce->selected() )
        needX11 = true;

    if ( modLibre->status() == MOD_INSTALL )
        needX11 = true;

    if ( needX11 && modX11->status() == MOD_DONT_INSTALL )
        selector->setItemStatus( modX11, MOD_AUTOINSTALL );

    if ( ! needX11 && modX11->status() == MOD_AUTOINSTALL )
        selector->setItemStatus( modX11, MOD_DONT_INSTALL );

    if ( needLibre && modLibre->status() == MOD_DONT_INSTALL )
        selector->setItemStatus( modLibre, MOD_AUTOINSTALL );

    if ( ! needLibre && modLibre->status() == MOD_AUTOINSTALL )
        selector->setItemStatus( modLibre, MOD_DONT_INSTALL );

    // selector->dumpItems();
}


void autoSelect( YItem * item )
{
    switch ( item->status() )
    {
        case MOD_DONT_INSTALL:
            selector->setItemStatus( item, MOD_AUTOINSTALL );
            break;

        case MOD_INSTALL:       // Manually selected; leave it alone
        case MOD_AUTOINSTALL:   // Already auto-selected; leave it alone
            break;
    }
}


void updateValueField()
{
    selector->dumpItems();
    YItem * item = selector->selectedItem();

    if ( item )
	valueField->setValue( item->label() );
    else
	valueField->setValue( "<none>" );
}


void updateResultField()
{
    string result = "";

    for ( YItem * item: selector->selectedItems() )
    {
	if ( ! result.empty() )
	    result += "\n";

	result += item->label();
    }

    resultField->setText( result );
}


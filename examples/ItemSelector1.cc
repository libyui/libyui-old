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


// Simple ItemSelector example.
//
// Compile with:
//
//     g++ -I/usr/include/yui -lyui ItemSelector1.cc -o ItemSelector1

#define YUILogComponent "example"
#include "YUILog.h"

#include "YUI.h"
#include "YWidgetFactory.h"
#include "YDialog.h"
#include "YLayoutBox.h"
#include "YItemSelector.h"
#include "YLabel.h"
#include "YPushButton.h"
#include "YAlignment.h"
#include "YEvent.h"


using std::string;

bool notify             = true;
bool singleSelection    = false;
bool selectorEnabled    = true;
int  visibleItems       = 4;


// Widgets

YDialog         * dialog        = 0;
YItemSelector	* selector      = 0;
YLabel	        * valueField    = 0;
YPushButton     * valueButton	= 0;
YLabel	        * resultField	= 0;
YPushButton     * resultButton	= 0;
YPushButton     * focusButton	= 0;
YPushButton     * enableButton	= 0;
YPushButton     * closeButton	= 0;


void createWidgets();
void handleEvents();
void updateValueField();
void updateResultField();


int main( int argc, char **argv )
{
    YUILog::setLogFileName( "/tmp/libyui-examples.log" );
    YUILog::enableDebugLogging();

    createWidgets();
    handleEvents();

    dialog->destroy();
}


void createWidgets()
{
    dialog	 = YUI::widgetFactory()->createPopupDialog();
    YAlignment * mbox	 = YUI::widgetFactory()->createMarginBox( dialog, 2, 2, 0.4, 0.4 );
    YLayoutBox * vbox	 = YUI::widgetFactory()->createVBox( mbox );

    YUI::widgetFactory()->createHeading( vbox, "Pizza Menu" );
    YUI::widgetFactory()->createVSpacing( vbox, 0.2 );

    selector = YUI::widgetFactory()->createItemSelector( vbox, singleSelection );
    YUI_CHECK_PTR( selector );

    YItemCollection items;
    items.push_back( new YDescribedItem( "Pizza Margherita",		"Very basic with just tomatoes and cheese"	) );
    items.push_back( new YDescribedItem( "Pizza Capricciosa",		"Ham and vegetables"				) );
    items.push_back( new YDescribedItem( "Pizza Funghi",		"Mushrooms"					) );
    items.push_back( new YDescribedItem( "Pizza Prosciutto",		"Ham"						) );
    items.push_back( new YDescribedItem( "Pizza Quattro Stagioni",	"Different toppings in each quarter"		) );
    items.push_back( new YDescribedItem( "Calzone",			"Folded over"					) );
    selector->addItems( items ); // This is more efficient than repeatedly calling selector->addItem()

    selector->setVisibleItems( visibleItems );

    if ( notify )
        selector->setNotify();

    YUI::widgetFactory()->createVSpacing( vbox, 0.4 );

    YLayoutBox * hbox1	= YUI::widgetFactory()->createHBox( vbox );
    valueField = YUI::widgetFactory()->createOutputField( hbox1, "<unknown>" );
    valueField->setStretchable( YD_HORIZ, true ); // allow stretching over entire dialog width

    valueButton = YUI::widgetFactory()->createPushButton( hbox1, "&Value" );

    YUI::widgetFactory()->createVSpacing( vbox, 0.3 );

    YLayoutBox * hbox2	 = YUI::widgetFactory()->createHBox( vbox );
    resultField = YUI::widgetFactory()->createOutputField( hbox2, "<selected items>\n\n\n\n\n" );
    resultField->setStretchable( YD_HORIZ, true ); // allow stretching over entire dialog width

    resultButton = YUI::widgetFactory()->createPushButton( hbox2, "&Selected\nItems" );

    YUI::widgetFactory()->createVSpacing( vbox, 0.3 );

    YAlignment	* alignment1	  = YUI::widgetFactory()->createLeft( vbox );
    YLayoutBox	* buttonBox	  = YUI::widgetFactory()->createHBox( alignment1 );
    focusButton	  = YUI::widgetFactory()->createPushButton( buttonBox, "Set &Focus" );
    enableButton	  = YUI::widgetFactory()->createPushButton( buttonBox, "&Enable"    );

    YAlignment	* rightAlignment  = YUI::widgetFactory()->createRight( vbox );
    closeButton	  = YUI::widgetFactory()->createPushButton( rightAlignment, "&Close" );
}


void handleEvents()
{
    while ( true )
    {
	YEvent * event = dialog->waitForEvent();

	if ( event )
	{
	    if ( event->eventType() == YEvent::CancelEvent ) // window manager "close window" button
		break; // leave event loop

	    if ( event->widget() == closeButton )
		break; // leave event loop

	    if ( event->widget() == focusButton )
	    {
		selector->setKeyboardFocus();
	    }

	    if ( event->widget() == enableButton )
	    {
		selectorEnabled = ! selectorEnabled;
		selector->setEnabled( selectorEnabled );

		// yuiMilestone() << "Enable: " << selectorEnabled << endl;
	    }

	    if ( event->widget() == resultButton )
		updateResultField();

	    if ( event->widget() == valueButton )
		updateValueField();

	    if ( event->widget() == selector )	   // the selector will only send events with setNotify()
	    {
		updateResultField();
		updateValueField();
	    }
	}
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

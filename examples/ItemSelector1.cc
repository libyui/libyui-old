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



int main( int argc, char **argv )
{
    YUILog::setLogFileName( "/tmp/libyui-examples.log" );
    YUILog::enableDebugLogging();

    //
    // Create and open dialog
    //

    YDialog    * dialog  = YUI::widgetFactory()->createPopupDialog();
    YLayoutBox * vbox    = YUI::widgetFactory()->createVBox( dialog );

    YItemSelector * selector = YUI::widgetFactory()->createItemSelector( vbox );
    YUI_CHECK_PTR( selector );

    YItemCollection items;
    items.push_back( new YDescribedItem( "Pizza Margherita" 	  ) );
    items.push_back( new YDescribedItem( "Pizza Capricciosa"	  ) );
    items.push_back( new YDescribedItem( "Pizza Funghi"		  ) );
    items.push_back( new YDescribedItem( "Pizza Prosciutto"	  ) );
    items.push_back( new YDescribedItem( "Pizza Quattro Stagioni" ) );
    items.push_back( new YDescribedItem( "Calzone"		  ) );
    selector->addItems( items ); // This is more efficient than repeatedly calling selector->addItem()


    YLayoutBox * hbox = YUI::widgetFactory()->createHBox( vbox );
    YLabel * valueField  = YUI::widgetFactory()->createOutputField( hbox, "<ItemSelector value unknown>" );
    valueField->setStretchable( YD_HORIZ, true ); // allow stretching over entire dialog width

    YPushButton * valueButton = YUI::widgetFactory()->createPushButton( hbox, "&Value" );

    YUI::widgetFactory()->createVSpacing( vbox, 0.3 );

    YAlignment  * rightAlignment = YUI::widgetFactory()->createRight( vbox );
    YPushButton * closeButton    = YUI::widgetFactory()->createPushButton( rightAlignment, "&Close" );


    //
    // Event loop
    //

    while ( true )
    {
	YEvent * event = dialog->waitForEvent();

	if ( event )
	{
	    if ( event->eventType() == YEvent::CancelEvent ) // window manager "close window" button
		break; // leave event loop

	    valueField->setValue( "???" );

	    if ( event->widget() == closeButton )
		break; // leave event loop

	    if ( event->widget() == valueButton ||
		 event->widget() == selector )		// selector will only send events with setNotify()
	    {
		YItem * item = selector->selectedItem();

		if ( item )
		    valueField->setValue( item->label() );
		else
		    valueField->setValue( "<none>" );
	    }
	}
    }


    //
    // Clean up
    //

    dialog->destroy();
}

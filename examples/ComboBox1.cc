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

// Simple ComboBox example.
//
// Compile with:
//
//     g++ -I/usr/include/yui -lyui ComboBox1.cc -o ComboBox1
// or
//     g++ -I/usr/include/yui -lyui -DEDITABLE ComboBox1.cc -o ComboBox1
//
// for an editable ComboBox.

#define YUILogComponent "example"
#include "YUILog.h"

#include "YUI.h"
#include "YWidgetFactory.h"
#include "YDialog.h"
#include "YLayoutBox.h"
#include "YComboBox.h"
#include "YLabel.h"
#include "YPushButton.h"
#include "YAlignment.h"
#include "YEvent.h"


#ifdef EDITABLE
    bool editable = true;
#else
    bool editable = false;
#endif





int main( int argc, char **argv )
{
    YUILog::setLogFileName( "/tmp/libyui-examples.log" );
    YUILog::enableDebugLogging();

    //
    // Create and open dialog
    //

    YDialog    * dialog  = YUI::widgetFactory()->createPopupDialog();
    YLayoutBox * vbox    = YUI::widgetFactory()->createVBox( dialog );

    YComboBox * comboBox = YUI::widgetFactory()->createComboBox( vbox, "&Menu", editable );

    YItemCollection items;
    items.push_back( new YItem( "Pizza Margherita" 	 ) );
    items.push_back( new YItem( "Pizza Capricciosa"	 ) );
    items.push_back( new YItem( "Pizza Funghi"		 ) );
    items.push_back( new YItem( "Pizza Prosciutto"	 ) );
    items.push_back( new YItem( "Pizza Quattro Stagioni" ) );
    items.push_back( new YItem( "Calzone"		 ) );
    comboBox->addItems( items ); // This is more efficient than repeatedly calling comboBox->addItem()


    YLayoutBox * hbox = YUI::widgetFactory()->createHBox( vbox );
    YLabel * valueField  = YUI::widgetFactory()->createOutputField( hbox, "<ComboBox value unknown>" );
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
		 event->widget() == comboBox )		// comboBox will only send events with setNotify()
	    {
		// Get the current value of the ComboBox and display it in valueField.
		//
		// In normal applications, it should be clear whether or not a
		// ComboBox is editable, so the following if()...else should
		// not be necessary, only either the "if" branch or the "else" branch.

		if ( comboBox->editable() )
		{
		    // An editable ComboBox can have a selectedItem(), but the user can
		    // also enter any other text. YComboBox::value() will return that text or,
		    // if the user chose an item from the list, that item's
		    // label.
		    //
		    // YComboBox::setValue() OTOH will automatically select a list item
		    // if an item with that label exists.
		    //
		    // Semantically, an editable ComboBox is an InputField with some
		    // frequently used values in a drop-down list. This means that it
		    // makes a lot more sense to use strings and YComboBox::value() /
		    // YComboBox::setValue() consistently for such a ComboBox, not
		    // YComboBox::selectedItem() / YComboBox::selectItem().

		    valueField->setValue( comboBox->value() );
		}
		else // not editable
		{
		    // For non-editable ComboBox widgets, using items is preferred:
		    // YComboBox::selectedItem() / YComboBox::selectItem().
		    //
		    // While it is possible to use YComboBox::value() and
		    // YComboBox::setValue(), this is not the idea behind it.

		    YItem * item = comboBox->selectedItem();

		    if ( item )
			valueField->setValue( item->label() );
		    else
			valueField->setValue( "<none>" );
		}
	    }
	}
    }


    //
    // Clean up
    //

    dialog->destroy();
}

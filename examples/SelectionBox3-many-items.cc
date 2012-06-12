// Performance stress test for SelectionBox: Add many items
//
// Replacing items, notify and immediate modes.
//
// Compile with:
//
//     g++ -I/usr/include/yui -lyui SelectionBox3-many-items.cc -o SelectionBox3-many-items

#define YUILogComponent "example"
#include "YUILog.h"

#include "YUI.h"
#include "YWidgetFactory.h"
#include "YDialog.h"
#include "YLayoutBox.h"
#include "YSelectionBox.h"
#include "YCheckBox.h"
#include "YLabel.h"
#include "YPushButton.h"
#include "YAlignment.h"
#include "YEvent.h"
#include <stdio.h>

#define ITEM_COUNT	10000

YItemCollection pizzaItems()
{
    YItemCollection items;
    yuiMilestone() << "Creating item collection" << std::endl;

    for ( int i=1; i<= ITEM_COUNT; i++ )
    {
	char name[80];
	sprintf( name, "Pizza #%05d", i );
	items.push_back( new YItem( name ) );
    }

    yuiMilestone() << "Item collection created" << std::endl;

    return items;
}


YItemCollection pastaItems()
{
    YItemCollection items;
    items.push_back( new YItem( "Spaghetti Napoli" 	 ) );
    items.push_back( new YItem( "Spaghetti Bolognese" 	 ) );
    items.push_back( new YItem( "Fusili Emiliana" 	 ) );
    items.push_back( new YItem( "Penne Calabrese" 	 ) );
    items.push_back( new YItem( "Penne Al Forno" 	 ) );
    items.push_back( new YItem( "Lasagne"	 	 ) );

    return items;
}



int main( int argc, char **argv )
{
    YUILog::setLogFileName( "/tmp/libyui-examples.log" );
    YUILog::enableDebugLogging();

    //
    // Create and open dialog
    //

    YDialog    * dialog  = YUI::widgetFactory()->createPopupDialog();
    YLayoutBox * vbox    = YUI::widgetFactory()->createVBox( dialog );

    // Specify larger size for the SelectionBox: It can scroll, so its size
    // depends on other widgets in the layout.
    YAlignment * minSize = YUI::widgetFactory()->createMinSize( vbox, 40, 8 ); // minWidth, minHeight


    // The SelectionBox. This is what this example all about.
    // The other widgets are just here to allow some experimenting.

    YSelectionBox * selBox = YUI::widgetFactory()->createSelectionBox( minSize, "&Menu" );
    selBox->addItems( pastaItems() );


    // Buttons to allow changing the SelectionBox items

    YLayoutBox  * buttonBox      = YUI::widgetFactory()->createHBox( vbox );
    YPushButton * pastaButton    = YUI::widgetFactory()->createPushButton( buttonBox, "&Pasta" );
    YPushButton * pizzaButton    = YUI::widgetFactory()->createPushButton( buttonBox, "Pi&zza" );
    YPushButton * clearButton    = YUI::widgetFactory()->createPushButton( buttonBox, "C&lear" );
    YPushButton * deselectButton = YUI::widgetFactory()->createPushButton( buttonBox, "&Deselect" );

    YUI::widgetFactory()->createVSpacing( vbox, 0.5 );


    // Check boxes to allow toogling "notify" and "immediate" for the SelectionBox

    YAlignment * leftAlignment = YUI::widgetFactory()->createLeft( vbox );
    YCheckBox * notifyCheckBox = YUI::widgetFactory()->createCheckBox( leftAlignment, "&Notify Mode" );
    notifyCheckBox->setNotify();

    leftAlignment = YUI::widgetFactory()->createLeft( vbox );
    YCheckBox * immediateCheckBox = YUI::widgetFactory()->createCheckBox( leftAlignment, "&Immediate Mode" );
    immediateCheckBox->setNotify();

    YUI::widgetFactory()->createVSpacing( vbox, 0.3 );


    // OutputField + button to allow monitoring the SelectionBox value
    // (and when it changes, i.e. when events are sent)

    YLayoutBox * hbox = YUI::widgetFactory()->createHBox( vbox );
    YLabel * valueField  = YUI::widgetFactory()->createOutputField( hbox, "???" );
    valueField->setStretchable( YD_HORIZ, true ); // allow stretching over entire dialog width

    YPushButton * valueButton = YUI::widgetFactory()->createPushButton( hbox, "&Value" );

    YUI::widgetFactory()->createVSpacing( vbox, 0.7 );


    // "Close" button

    YAlignment    * rightAlignment = YUI::widgetFactory()->createRight( vbox );
    YPushButton   * closeButton    = YUI::widgetFactory()->createPushButton( rightAlignment, "&Close" );


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
	    else if ( event->widget() == pastaButton )
	    {
		selBox->deleteAllItems();
		selBox->addItems( pastaItems() );
	    }
	    else if ( event->widget() == pizzaButton )
	    {
		selBox->deleteAllItems();
		selBox->addItems( pizzaItems() );
		yuiMilestone() << "Pizza items added" << std::endl;
	    }
	    else if ( event->widget() == clearButton )
	    {
		selBox->deleteAllItems();
	    }
	    else if ( event->widget() == deselectButton )
	    {
		selBox->deselectAllItems();
	    }
	    else if ( event->widget() == notifyCheckBox )
	    {
		bool notify = notifyCheckBox->isChecked();

		if ( ! notify )
		{
		    // immediateMode implicitly includes notify, so set
		    // immediateMode off if the user wants to set notify off

		    selBox->setImmediateMode( false );
		    immediateCheckBox->setChecked( false );
		}

		selBox->setNotify( notify );
	    }
	    else if ( event->widget() == immediateCheckBox )
	    {
		bool immediate = immediateCheckBox->isChecked();
		selBox->setImmediateMode( immediate );

		// immediateMode implicitly includes notify;
		// reflect this in the notify check box

		if ( immediate )
		    notifyCheckBox->setChecked( true );
	    }
	    else if ( event->widget() == selBox ||
		      event->widget() == valueButton )
	    {
		yuiMilestone() << "Event widget: " << event->widget() << std::endl;

		YItem * item = selBox->selectedItem();

		if ( item )
		    valueField->setValue( item->label() );
		else
		    valueField->setValue( "<none>" );
	    }
	    else
	    {
		yuiMilestone() << "Unknown event" << std::endl;
	    }
	}
    }


    //
    // Clean up
    //

    dialog->destroy();
}

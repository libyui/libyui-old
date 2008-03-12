// SelectionBox example.
//
// Compile with:
//
//     g++ -I/usr/include/YaST2/yui -lyui SelectionBox1.cc -o SelectionBox1

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


YItemCollection pizzaItems()
{
    YItemCollection items;
    items.push_back( new YItem( "Pizza Margherita" 	 ) );
    items.push_back( new YItem( "Pizza Capricciosa"	 ) );
    items.push_back( new YItem( "Pizza Funghi"		 ) );
    items.push_back( new YItem( "Pizza Prosciutto"	 ) );
    items.push_back( new YItem( "Pizza Quattro Stagioni" ) );
    items.push_back( new YItem( "Calzone"		 ) );

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
    //
    // Create and open dialog
    //

    YDialog    * dialog  = YUI::widgetFactory()->createPopupDialog();
    YLayoutBox * vbox    = YUI::widgetFactory()->createVBox( dialog );

    // Specify larger size for the SelectionBox: It can scroll, so its size
    // depends on other widgets in the layout.
    YAlignment * minSize = YUI::widgetFactory()->createMinSize( vbox, 40, 8 ); // minWidth, minHeight

    //
    // This is what this example all about.
    // The other widgets are here just to allow some experimenting.
    //

    YSelectionBox * selBox = YUI::widgetFactory()->createSelectionBox( minSize, "&Menu" );
    selBox->addItems( pizzaItems() );

    YLayoutBox    * buttonBox	= YUI::widgetFactory()->createHBox( vbox );
    YPushButton   * pastaButton = YUI::widgetFactory()->createPushButton( buttonBox, "&Pasta" );
    YPushButton   * pizzaButton = YUI::widgetFactory()->createPushButton( buttonBox, "Pi&zza" );

    YUI::widgetFactory()->createVSpacing( vbox, 0.5 );

    YAlignment * leftAlignment = YUI::widgetFactory()->createLeft( vbox );
    YCheckBox * notifyCheckBox = YUI::widgetFactory()->createCheckBox( leftAlignment, "&Notify Mode" );
    notifyCheckBox->setNotify();

    leftAlignment = YUI::widgetFactory()->createLeft( vbox );
    YCheckBox * immediateCheckBox = YUI::widgetFactory()->createCheckBox( leftAlignment, "&Immediate Mode" );
    immediateCheckBox->setNotify();

    YUI::widgetFactory()->createVSpacing( vbox, 0.3 );

    YLayoutBox * hbox = YUI::widgetFactory()->createHBox( vbox );
    YLabel * valueField  = YUI::widgetFactory()->createOutputField( hbox, "???" );
    valueField->setStretchable( YD_HORIZ, true ); // allow stretching over entire dialog width

    YPushButton * valueButton = YUI::widgetFactory()->createPushButton( hbox, "&Value" );

    YUI::widgetFactory()->createVSpacing( vbox, 0.7 );

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

	    YWidgetEvent * widgetEvent = dynamic_cast<YWidgetEvent *> (event);

	    if ( widgetEvent )
	    {
		valueField->setValue( "???" );

		if ( widgetEvent->widget() == closeButton )
		    break; // leave event loop
		else if ( widgetEvent->widget() == pastaButton )
		{
		    selBox->deleteAllItems();
		    selBox->addItems( pastaItems() );
		}
		else if ( widgetEvent->widget() == pizzaButton )
		{
		    selBox->deleteAllItems();
		    selBox->addItems( pizzaItems() );
		}
		else if ( widgetEvent->widget() == notifyCheckBox )
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
		else if ( widgetEvent->widget() == immediateCheckBox )
		{
		    bool immediate = immediateCheckBox->isChecked();
		    selBox->setImmediateMode( immediate );

		    // immediateMode implicitly includes notify;
		    // reflect this in the notify check box

		    if ( immediate )
			notifyCheckBox->setChecked( true );
		}
		else if ( widgetEvent->widget() == selBox ||
			  widgetEvent->widget() == valueButton )
		{
		    YItem * item = selBox->selectedItem();

		    if ( item )
			valueField->setValue( item->label() );
		    else
			valueField->setValue( "<none>" );
		}
	    }


	    // YDialog::waitForEvent() allocates a new YEvent and transfers
	    // ownership of the event to the caller, so the caller has to make
	    // sure the memory for the event is deallocated after use.
	    // Otherwise there will be a memory leak.

	    delete event;
	}
    }


    //
    // Clean up
    //

    dialog->destroy();
}

// Trivial libyui example.
//
// Compile with:
//
//     g++ -I/usr/include/YaST2/yui -lyui HelloWorld.cc -o HelloWorld

#include "YUI.h"
#include "YWidgetFactory.h"
#include "YDialog.h"
#include "YLayoutBox.h"
#include "YEvent.h"

int main( int argc, char **argv )
{
    YDialog    * dialog = YUI::widgetFactory()->createPopupDialog();
    YLayoutBox * vbox   = YUI::widgetFactory()->createVBox( dialog );
    YUI::widgetFactory()->createLabel     ( vbox, "Hello, World!" );
    YUI::widgetFactory()->createPushButton( vbox, "&OK" );
    
    dialog->waitForEvent();
    dialog->destroy();
}

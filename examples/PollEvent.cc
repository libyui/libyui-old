// Copyright (c) [2019] SUSE LLC
//
// All Rights Reserved.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of version 2 of the GNU General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, contact SUSE LLC.
//
// To contact SUSE LLC about this file by physical or electronic mail, you may
// find current contact information at www.suse.com.


// libyui example for pollEvent
//
// Compile with:
//
//     g++ -I/usr/include/yui -lyui PollEvent.cc -o PollEvent


#define YUILogComponent "example"
#include "YUILog.h"

#include "YUI.h"
#include "YWidgetFactory.h"
#include "YDialog.h"
#include "YLayoutBox.h"
#include "YLabel.h"
#include "YPushButton.h"
#include "YEvent.h"

int main( int argc, char **argv )
{
    YDialog    * dialog       = YUI::widgetFactory()->createPopupDialog();
    YLayoutBox * vbox         = YUI::widgetFactory()->createVBox( dialog );
    YLabel *      label       = YUI::widgetFactory()->createLabel     ( vbox, "Event #0" );
    YPushButton * doButton    = YUI::widgetFactory()->createPushButton( vbox, "&Do Something" );
    YPushButton * closeButton = YUI::widgetFactory()->createPushButton( vbox, "&Close" );
    int eventCount = 0;

    while ( true )
    {
        YEvent * event = dialog->pollEvent();

        if ( ! event )
            continue;

        if ( event->eventType() == YEvent::CancelEvent ) // window manager "close window" button
            break; // leave event loop

        if ( event->widget() == closeButton )
            break; // leave event loop
        
        if ( event->widget() == doButton )
        {
            yuiMilestone() << "DoSomething button pressed" << std::endl;
        }

        ++eventCount;
        
        char msg[80];
        sprintf( msg, "Event #%d", eventCount );
        label->setText( msg );
    }
    
    dialog->destroy();
}

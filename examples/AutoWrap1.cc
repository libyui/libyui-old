/*
  Copyright (c) [2020] SUSE LLC
  
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



// Libyui example for auto-wrapping labels.
//
// Local Variables:
//    compile-command: "g++ -I/usr/include/yui -lyui AutoWrap1.cc -o AutoWrap1"
// End:

#define YUILogComponent "example"
#include "YUILog.h"

#include "YUI.h"
#include "YWidgetFactory.h"
#include "YDialog.h"
#include "YLayoutBox.h"
#include "YAlignment.h"
#include "YLabel.h"
#include "YEvent.h"

#include "LoremIpsum.h"

using std::string;


int main( int argc, char **argv )
{
    YUILog::setLogFileName( "/tmp/libyui-examples.log" );
    YUILog::enableDebugLogging();

    YWidgetFactory * fac = YUI::widgetFactory();
    YDialog    * dialog  = fac->createPopupDialog();
    YLayoutBox * vbox    = fac->createVBox( dialog );
    YAlignment * minSize = fac->createMinSize( vbox, 74, 18 );
    YLabel     * label   = fac->createLabel( minSize, loremIpsum() );
    label->setAutoWrap();
    YUI::widgetFactory()->createPushButton( vbox, "&OK" );

    dialog->waitForEvent();
    dialog->destroy();
}

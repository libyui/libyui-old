/*
  Copyright (c) 2016 SUSE LCC

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
  SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
  THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


// Menu Button example demonstrating also shortcut resolver.
//
// Compile with:
//
//     g++ -I/usr/include/yui -lyui MenuButton1.cc -o MenuButton1

#include "YUI.h"
#include "YWidgetFactory.h"
#include "YDialog.h"
#include "YLayoutBox.h"
#include "YEvent.h"
#include "YMenuButton.h"
#include "YMenuItem.h"

int main( int argc, char **argv )
{
    YDialog    * dialog = YUI::widgetFactory()->createPopupDialog();
    YLayoutBox * vbox   = YUI::widgetFactory()->createVBox( dialog );
    YUI::widgetFactory()->createLabel( vbox, "Hello, World!" );
    YMenuButton* top = YUI::widgetFactory()->createMenuButton( vbox, "Menu!" );
    YMenuItem *inner_item = new YMenuItem("&menu1");
    top->addItem(inner_item);
    top->addItem(new YMenuItem("&menu2"));
    top->addItem(new YMenuItem("&Menu3")); // test upper letter as shortcut
    top->addItem(new YMenuItem("menu4")); // even without shortcut marker it should find shortcut if possible
    new YMenuItem(inner_item, "&submenu1");
    new YMenuItem(inner_item, "&submenu2");
    new YMenuItem(inner_item, "&submenu3");
    new YMenuItem(inner_item, "&submenu4");

    top->resolveShortcutConflicts();
    top->rebuildMenuTree();

    dialog->waitForEvent();
    dialog->destroy();
}

/*
  Copyright (C) 2000-2012 Novell, Inc
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


/*-/

  File:		YDialogHelpers.cc

  Author:	Jiri Srain <jsrain@suse.cz>

/-*/


#include <sys/types.h>
#include <dirent.h>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YDialog.h"
#include "YEvent.h"
#include "YPushButton.h"
#include "YButtonBox.h"

#include "YUI.h"
#include "YApplication.h"
#include "YWidgetFactory.h"
#include "YOptionalWidgetFactory.h"
#include "YLayoutBox.h"
#include "YRichText.h"
#include "YAlignment.h"
#include "YUIException.h"
#include "YEventFilter.h"
#include "YWidgetID.h"
#include "YDumbTab.h"


using std::string;
using std::map;
using std::vector;


void
YDialog::showText( const string & text, bool useRichText )
{

    // set help text dialog size to 80% of topmost dialog, respectively 45x15 (default)

    unsigned int dialogWidth  = 45;
    unsigned int dialogHeight = 15;

    if ( ! _dialogStack.empty() )
    {
	YDialog * dialog = _dialogStack.top();
        dialogWidth  = (unsigned int) ( (float) dialog->preferredWidth()  * 0.8 );
        dialogHeight = (unsigned int) ( (float) dialog->preferredHeight() * 0.8 );
    }

    // limit dialog to a reasonable size
    if ( dialogWidth > 80 || dialogHeight > 25 )
    {
	dialogWidth = 80;
	dialogHeight = 25;
    }

    try
    {
	YDialog     * dialog    = YUI::widgetFactory()->createPopupDialog();
	YAlignment  * minSize   = YUI::widgetFactory()->createMinSize( dialog, dialogWidth, dialogHeight );
	YLayoutBox  * vbox      = YUI::widgetFactory()->createVBox( minSize );
	YUI::widgetFactory()->createRichText( vbox, text, ! useRichText );
	YButtonBox  * buttonBox = YUI::widgetFactory()->createButtonBox( vbox );
	YPushButton * okButton  = YUI::widgetFactory()->createPushButton( buttonBox, "&OK" );
	okButton->setRole( YOKButton );
	okButton->setDefaultButton();

	dialog->waitForEvent();
	dialog->destroy();
    }
    catch ( const YUIException & exception )
    {
	// Don't let the application die just because help couldn't be displayed.

	YUI_CAUGHT( exception );
    }
}


bool
YDialog::showHelpText( YWidget * widget )
{
    string helpText;

    while ( widget )
    {
	if ( ! widget->helpText().empty() )
	{
	    yuiDebug() << "Found help text for " << widget << endl;
	    helpText = widget->helpText();
	}

	widget = widget->parent();
    }

    if ( ! helpText.empty() )
    {
	yuiMilestone() << "Showing help text" << endl;
	showText( helpText, true );

	yuiMilestone() << "Help dialog closed" << endl;
    }
    else // No help text
    {
	yuiWarning() << "No help text" << endl;
    }

    return ! helpText.empty();
}


bool
YDialog::showRelNotesText()
{
    yuiMilestone() <<"Showing Release Notes" << endl;

    // set help text dialog size to 80% of topmost dialog, respectively 45x15 (default)

    unsigned int dialogWidth  = 45;
    unsigned int dialogHeight = 15;

    if ( ! _dialogStack.empty() )
    {
        YDialog * dialog = _dialogStack.top();
        dialogWidth  = (unsigned int) ( (float) dialog->preferredWidth()  * 0.8 );
        dialogHeight = (unsigned int) ( (float) dialog->preferredHeight() * 0.8 );
    }

    // limit dialog to a reasonable size
    if ( dialogWidth > 80 || dialogHeight > 25 )
    {
        dialogWidth = 80;
        dialogHeight = 25;
    }

    try
    {
	map<string,string> relnotes = YUI::application()->releaseNotes();

	if ( relnotes.size() == 0)
	{
	    return false;
	}

	vector<string> keys;
	for ( map<string,string>::iterator it = relnotes.begin(); it != relnotes.end(); ++it )
        {
	    keys.push_back(it->first);
	}
        YDialog     * dialog    = YUI::widgetFactory()->createPopupDialog();
        YAlignment  * minSize   = YUI::widgetFactory()->createMinSize( dialog, dialogWidth, dialogHeight );
        YLayoutBox  * vbox      = YUI::widgetFactory()->createVBox( minSize );
        YDumbTab    * rnTab     = 0;
        YRichText   * richtext  = 0;

	// both QT and NCurses do support DumbTab
        if ( relnotes.size() > 1 && YUI::optionalWidgetFactory()->hasDumbTab() )
	{
	    rnTab = YUI::optionalWidgetFactory()->createDumbTab( vbox );
	    int index = 0;
	    for(map<string,string>::const_iterator it = relnotes.begin(); it != relnotes.end(); it++)
	    {
		YItem * item = new YItem((*it).first );
		item->setIndex( index++ );
		rnTab->addItem( item );
	    }
	    richtext = YUI::widgetFactory()->createRichText( rnTab, (*(relnotes.begin())).second, YUI::app()->isTextMode() );
	}
	else
	{
	    richtext = YUI::widgetFactory()->createRichText( vbox, (*(relnotes.begin())).second, YUI::app()->isTextMode() );
	}

        YButtonBox  * buttonBox = YUI::widgetFactory()->createButtonBox( vbox );
        YPushButton * okButton  = YUI::widgetFactory()->createPushButton( buttonBox, "&OK" );
        okButton->setRole( YOKButton );
        okButton->setDefaultButton();

	while(true) {
	    YEvent* event = dialog->waitForEvent();
	    if ( event && event->eventType() == YEvent::MenuEvent && event->item())
	    {
		YItem * item = dynamic_cast<YItem *> ( event->item());
		richtext->setValue( relnotes[keys[item->index()]] );
	    }
	    else if ( event && event->widget() )
	    {
		YPushButton * button = dynamic_cast<YPushButton *> ( event->widget() );
		if ( button )
		{
		    if ( button->role() == YOKButton)
		    {
			break;
		    }
		}
            }
	}
        dialog->destroy();
    }
    catch ( const YUIException & exception )
    {
        // Don't let the application die just because the release notes couldn't be displayed.

        YUI_CAUGHT( exception );
    }

    return true;
}

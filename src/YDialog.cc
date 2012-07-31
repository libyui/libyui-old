/*
  Copyright (C) 1970-2012 Novell, Inc
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

  File:		YDialog.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <list>
#include <algorithm>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YDialog.h"
#include "YEvent.h"
#include "YShortcutManager.h"
#include "YPushButton.h"
#include "YButtonBox.h"

#include "YUI.h"
#include "YWidgetFactory.h"
#include "YLayoutBox.h"
#include "YRichText.h"
#include "YAlignment.h"
#include "YUIException.h"
#include "YEventFilter.h"


#define VERBOSE_DIALOGS			0
#define VERBOSE_DISCARDED_EVENTS	0
#define VERBOSE_EVENTS			0


typedef std::list<YEventFilter *> YEventFilterList;


struct YDialogPrivate
{
    YDialogPrivate( YDialogType dialogType, YDialogColorMode colorMode )
	: dialogType( dialogType )
	, colorMode( colorMode )
	, shortcutCheckPostponed( false )
	, defaultButton( 0 )
	, isOpen( false )
	, lastEvent( 0 )
	{}

    YDialogType		dialogType;
    YDialogColorMode	colorMode;
    bool		shortcutCheckPostponed;
    YPushButton *	defaultButton;
    bool		isOpen;
    YEvent *		lastEvent;
    YEventFilterList	eventFilterList;
};



/**
 * Helper class: Event filter that handles "Help" buttons.
 **/
class YHelpButtonHandler: public YEventFilter
{
public:
    YHelpButtonHandler( YDialog * dialog )
	: YEventFilter( dialog )
	{}

    virtual ~YHelpButtonHandler() {}

    YEvent * filter( YEvent * event )
    {
	if ( event && event->widget() )
	{
	    YPushButton * button = dynamic_cast<YPushButton *> ( event->widget() );

	    if ( button && button->isHelpButton() )
	    {
		if ( YDialog::showHelpText( button ) )
		{
		    event = 0; // consume event
		}
	    }
	}

	return event;
    }
};




YDialog::YDialog( YDialogType dialogType, YDialogColorMode colorMode )
    : YSingleChildContainerWidget( 0 )
    , priv( new YDialogPrivate( dialogType, colorMode ) )
{
    YUI_CHECK_NEW( priv );

    _dialogStack.push( this );

#if VERBOSE_DIALOGS
    yuiDebug() << "New " << this << std::endl;
#endif

    new YHelpButtonHandler( this );
}


YDialog::~YDialog()
{
#if VERBOSE_DIALOGS
    yuiDebug() << "Destroying " << this << std::endl;
#endif

    // Inform attached classes that this dialog is in the process of being
    // destroyed. This also happens in the base class destructor, but that
    // might be too late.
    setBeingDestroyed();

    if ( priv->lastEvent )
	deleteEvent( priv->lastEvent );

    // The base class also deletes all children, but this should be done before
    // the event filters are deleted to prevent duplicate event filter deletion
    // from (a) child widget destructors and (b) here.
    deleteChildren();

    // Delete the remaining event filters: Those installed by this dialog and
    // those installed by some child widget that are not deleted yet.
    deleteEventFilters();

    if ( ! _dialogStack.empty() && _dialogStack.top() == this )
    {
	_dialogStack.pop();

	if ( ! _dialogStack.empty() )
	    _dialogStack.top()->activate();
    }
    else
	yuiError() << "Not top of dialog stack: " << this << std::endl;
}


void
YDialog::open()
{
    if ( priv->isOpen )
	return;

    checkShortcuts();
    setInitialSize();
    openInternal();	// Make sure this is only called once!

    priv->isOpen = true;
}


bool
YDialog::isOpen() const
{
    return priv->isOpen;
}


bool
YDialog::isTopmostDialog() const
{
    if ( _dialogStack.empty() )
    {
	yuiError() << "Dialog stack empty, but dialog existing: " << this << std::endl;
	return false;
    }

    return _dialogStack.top() == this;
}


void
YDialog::deleteEventFilters()
{
    while ( ! priv->eventFilterList.empty() )
    {
	YEventFilter * filter = priv->eventFilterList.back();

#if VERBOSE_DIALOGS
	yuiDebug() << "Deleting event filter " << std::std::hex << filter << std::dec << std::endl;
#endif
	delete filter;
    }
}


bool
YDialog::destroy( bool doThrow )
{
    YUI_CHECK_WIDGET( this );

    if ( isTopmostDialog() )
    {
	delete this;

	return true;
    }
    else
    {
	if ( doThrow )
	    YUI_THROW( YUIDialogStackingOrderException() );

	return false;
    }
}


YDialogType
YDialog::dialogType() const
{
    return priv->dialogType;
}


bool
YDialog::isMainDialog()
{
    switch ( priv->dialogType )
    {
	case YMainDialog:	return true;
	case YWizardDialog:	return true;
	case YPopupDialog:	return false;

	    // Intentionally omitting the 'default' case so the compiler can
	    // catch unhandled enum values
    }

    /*NOTREACHED*/
    return false;
}


YDialogColorMode
YDialog::colorMode() const
{
    return priv->colorMode;
}


void
YDialog::postponeShortcutCheck()
{
    priv->shortcutCheckPostponed = true;
}


bool
YDialog::shortcutCheckPostponed() const
{
    return priv->shortcutCheckPostponed;
}


void
YDialog::checkShortcuts( bool force )
{
    if ( priv->shortcutCheckPostponed && ! force )
    {
	yuiDebug() << "Shortcut check postponed" << std::endl;
    }
    else
    {

	YShortcutManager shortcutManager( this );
	shortcutManager.checkShortcuts();

	priv->shortcutCheckPostponed = false;
    }
}


YPushButton *
YDialog::defaultButton() const
{
    return priv->defaultButton;
}


void
YDialog::setDefaultButton( YPushButton * newDefaultButton )
{
    if ( newDefaultButton && priv->defaultButton ) // already have one?
    {
	yuiError() << "Too many `opt(`default) PushButtons: ["
		   << newDefaultButton->label()
		   << "]" << std::endl;
    }

    priv->defaultButton = newDefaultButton;
}


void
YDialog::setInitialSize()
{
#if VERBOSE_DIALOGS
    yuiDebug() << "Setting initial size for " << this << std::endl;
#endif

    // Trigger geometry management
    setSize( preferredWidth(), preferredHeight() );
}


void
YDialog::recalcLayout()
{
    yuiDebug() << "Recalculating layout for " << this << std::endl;

    setSize( preferredWidth(), preferredHeight() );
}


YEvent *
YDialog::waitForEvent( int timeout_millisec )
{
    if ( ! isTopmostDialog() )
	YUI_THROW( YUIDialogStackingOrderException() );

    if ( timeout_millisec < 0 )
	timeout_millisec = 0;

    if ( ! isOpen() )
	open();

    if ( shortcutCheckPostponed() )
    {
	yuiError() << "Performing missing keyboard shortcut check now in "
		   << this << std::endl;

	checkShortcuts( true );
    }

    deleteEvent( priv->lastEvent );
    YEvent * event = 0;

    do
    {
	event = filterInvalidEvents( waitForEventInternal( timeout_millisec ) );
	event = callEventFilters( event );

	// If there was no event, if filterInvalidEvents() discarded an invalid
	// event, or if one of the event filters consumed an event, go back and
	// get the next event.

    } while ( ! event );

    priv->lastEvent = event;

    return event;
}


YEvent *
YDialog::pollEvent()
{
    if ( ! isTopmostDialog() )
	YUI_THROW( YUIDialogStackingOrderException() );

    if ( ! isOpen() )
	open();

    YEvent * event = filterInvalidEvents( pollEventInternal() );

    if ( event ) // Optimization (calling with 0 wouldn't hurt)
	event = callEventFilters( event );

    priv->lastEvent = event;

    // Nevermind if filterInvalidEvents() discarded an invalid event.
    // pollInput() is normally called very often (in a loop), and most of the
    // times it returns 0 anyway, so there is no need to care for just another
    // 0 that is returned in this exotic case.

    return event;
}


YEvent *
YDialog::filterInvalidEvents( YEvent * event )
{
    if ( ! event )
	return 0;

    YWidgetEvent * widgetEvent = dynamic_cast<YWidgetEvent *> (event);

    if ( widgetEvent && widgetEvent->widget() )
    {
	if ( ! widgetEvent->widget()->isValid() )
	{
	    /**
	     * Silently discard events from widgets that have become invalid.
	     *
	     * This may legitimately happen if some widget triggered an event yet
	     * nobody cared for that event (i.e. called UserInput() or PollInput() )
	     * and the widget has been destroyed meanwhile.
	     **/

	    // yuiDebug() << "Discarding event for widget that has become invalid" << std::endl;

	    deleteEvent( widgetEvent );
	    return 0;
	}

	if ( widgetEvent->widget()->findDialog() != this )
	{
	    /**
	     * Silently discard events from all but the current (topmost) dialog.
	     *
	     * This may happen even here even though the specific UI should have
	     * taken care about that: Events may still be in the queue. They might
	     * have been valid (i.e. belonged to the topmost dialog) when they
	     * arrived, but maybe simply nobody has evaluated them.
	     **/

	    // Yes, really yuiDebug() - this may legitimately happen.
	    yuiDebug() << "Discarding event from widget from foreign dialog" << std::endl;

#if VERBOSE_DISCARDED_EVENTS
	    yuiDebug() << "Expected: "   << this
		       << ", received: " << widgetEvent->widget()->findDialog()
		       << std::endl;

	    yuiDebug() << "Event widget: "  << widgetEvent->widget() << std::endl;
	    yuiDebug() << "From:" << std::endl;
	    widgetEvent->widget()->findDialog()->dumpWidgetTree();
	    yuiDebug() << "Current dialog:" << std::endl;
	    dumpWidgetTree();
#endif

	    activate(); // try to force this dialog to the foreground

	    deleteEvent( widgetEvent );
	    return 0;
	}

    }

    return event;
}


void
YDialog::deleteEvent( YEvent * event )
{
    if ( event == priv->lastEvent )
	priv->lastEvent = 0;

    if ( event )
    {
	if ( event->isValid() )
	{
#if VERBOSE_EVENTS
	    yuiDebug() << "Deleting " << event << std::endl;
#endif
	    delete event;
	}
	else
	{
	    yuiError() << "Attempt to delete invalid event " << event << std::endl;
	}
    }
}


YDialog *
YDialog::currentDialog( bool doThrow )
{
    if ( _dialogStack.empty() )
    {
	if ( doThrow )
	    YUI_THROW( YUINoDialogException() );
	return 0;
    }
    else
	return _dialogStack.top();
}


bool
YDialog::deleteTopmostDialog( bool doThrow )
{
    if ( _dialogStack.empty() )
    {
	if ( doThrow )
	    YUI_THROW( YUINoDialogException() );
    }
    else
    {
	delete _dialogStack.top();
    }

    return ! _dialogStack.empty();
}


void
YDialog::deleteAllDialogs()
{
    while ( ! _dialogStack.empty() )
    {
	delete _dialogStack.top();
    }
}


void
YDialog::deleteTo( YDialog * targetDialog )
{
    YUI_CHECK_WIDGET( targetDialog );

    while ( ! _dialogStack.empty() )
    {
	YDialog * dialog = _dialogStack.top();

	delete dialog;

	if ( dialog == targetDialog )
	    return;
    }

    // If we ever get here, targetDialog was nowhere in the dialog stack.

    YUI_THROW( YUIDialogStackingOrderException() );
}


int
YDialog::openDialogsCount()
{
    return _dialogStack.size();
}


void
YDialog::addEventFilter( YEventFilter * eventFilter )
{
    YUI_CHECK_PTR( eventFilter );

    if ( find( priv->eventFilterList.begin(), priv->eventFilterList.end(),
	       eventFilter ) != priv->eventFilterList.end() )
    {
	yuiError() << "event filter " << std::hex << eventFilter << std::dec
		   << " already added to " << this
		   << std::endl;
    }
    else
    {
#if VERBOSE_DIALOGS
	yuiDebug() << "Adding event filter " << std::hex << eventFilter << std::dec << std::endl;
#endif
	priv->eventFilterList.push_back( eventFilter );
    }
}


void
YDialog::removeEventFilter( YEventFilter * eventFilter )
{
    YUI_CHECK_PTR( eventFilter );

#if VERBOSE_DIALOGS
    yuiDebug() << "Removing event filter " << std::hex << eventFilter << std::dec << std::endl;
#endif
    priv->eventFilterList.remove( eventFilter );
}


YEvent *
YDialog::callEventFilters( YEvent * event )
{
    YEventFilterList::const_iterator it = priv->eventFilterList.begin();

    while ( it != priv->eventFilterList.end() && event )
    {
	YEvent * oldEvent = event;
	event = (*it)->filter( event );

	if ( oldEvent != event )     // event filter consumed or changed the old event?
	    deleteEvent( oldEvent ); // get rid of the old one

	++it;
    }

    return event;
}


void
YDialog::showText( const std::string & text, bool useRichText )
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
    catch ( YUIException exception )
    {
	// Don't let the application die just because help couldn't be displayed.

	YUI_CAUGHT( exception );
    }
}


bool
YDialog::showHelpText( YWidget * widget )
{
    std::string helpText;

    while ( widget )
    {
	if ( ! widget->helpText().empty() )
	{
	    yuiDebug() << "Found help text for " << widget << std::endl;
	    helpText = widget->helpText();
	}

	widget = widget->parent();
    }

    if ( ! helpText.empty() )
    {
	yuiMilestone() << "Showing help text" << std::endl;
	showText( helpText, true );

	yuiMilestone() << "Help dialog closed" << std::endl;
    }
    else // No help text
    {
	yuiWarning() << "No help text" << std::endl;
    }

    return ! helpText.empty();
}

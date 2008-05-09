/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:		YDialog.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YDialog.h"
#include "YEvent.h"
#include "YShortcutManager.h"
#include "YPushButton.h"

#include "YUI.h"
#include "YWidgetFactory.h"
#include "YLayoutBox.h"
#include "YRichText.h"
#include "YAlignment.h"
#include "YUIException.h"

using std::string;

#define VERBOSE_DIALOGS			0
#define VERBOSE_DISCARDED_EVENTS	0


std::stack<YDialog *> YDialog::_dialogStack;


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
};



YDialog::YDialog( YDialogType dialogType, YDialogColorMode colorMode )
    : YSingleChildContainerWidget( 0 )
    , priv( new YDialogPrivate( dialogType, colorMode ) )
{
    YUI_CHECK_NEW( priv );

    _dialogStack.push( this );

#if VERBOSE_DIALOGS
    yuiDebug() << "New " << this << endl;
#endif
}


YDialog::~YDialog()
{
#if VERBOSE_DIALOGS
    yuiDebug() << "Destroying " << this << endl;
#endif

    if ( priv->lastEvent )
	deleteEvent( priv->lastEvent );

    if ( ! _dialogStack.empty() && _dialogStack.top() == this )
    {
	_dialogStack.pop();

	if ( ! _dialogStack.empty() )
	    _dialogStack.top()->activate();
    }
    else
	yuiError() << "Not top of dialog stack: " << this << endl;
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
	yuiError() << "Dialog stack empty, but dialog existing: " << this << endl;
	return false;
    }

    return _dialogStack.top() == this;
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
	yuiDebug() << "Shortcut check postponed" << endl;
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
		   << "]" << endl;
    }

    priv->defaultButton = newDefaultButton;
}


void
YDialog::setInitialSize()
{
#if VERBOSE_DIALOGS
    yuiDebug() << "Setting initial size for " << this << endl;
#endif

    // Trigger geometry management
    setSize( preferredWidth(), preferredHeight() );
}


void
YDialog::recalcLayout()
{
    yuiDebug() << "Recalculating layout for " << this << endl;

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
		   << this << endl;

	checkShortcuts( true );
    }

    if ( priv->lastEvent )
	deleteEvent( priv->lastEvent );

    YEvent * event = 0;

    do
    {
	event = filterInvalidEvents( waitForEventInternal( timeout_millisec ) );

	// If there was no event or if filterInvalidEvents() discarded
	// an invalid event, go back and get the next one.

	if ( event && event->widget() )
	{
	    YPushButton * button = dynamic_cast<YPushButton *> ( event->widget() );

	    if ( button && button->isHelpButton() )
	    {
		string helpText;
		YWidget * widget = button;

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

		    // Don't return the event from this help button -
		    // get back into event loop
		    deleteEvent( event );
		    event = 0;

		    yuiMilestone() << "Help dialog closed" << endl;
		}
		else // No help text
		{
		    // Return the event from this help button
		    yuiWarning() << "No help text in " << this << endl;
		}
	    }
	}

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

	    // yuiDebug() << "Discarding event for widget that has become invalid" << endl;

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
	    yuiDebug() << "Discarding event from widget from foreign dialog" << endl;

#if VERBOSE_DISCARDED_EVENTS
	    yuiDebug() << "Expected: "   << this
		       << ", received: " << widgetEvent->widget()->findDialog()
		       << endl;

	    yuiDebug() << "Event widget: "  << widgetEvent->widget() << endl;
	    yuiDebug() << "From:" << endl;
	    widgetEvent->widget()->findDialog()->dumpWidgetTree();
	    yuiDebug() << "Current dialog:" << endl;
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
	delete event;
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
YDialog::showText( const string & text, bool useRichText )
{
    try
    {
	YDialog     * dialog   = YUI::widgetFactory()->createPopupDialog();
	YAlignment  * minSize  = YUI::widgetFactory()->createMinSize( dialog, 45, 15 );
	YLayoutBox  * vbox     = YUI::widgetFactory()->createVBox( minSize );
	YUI::widgetFactory()->createRichText( vbox, text, ! useRichText );
	YPushButton * okButton = YUI::widgetFactory()->createPushButton( vbox, "&OK" );
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

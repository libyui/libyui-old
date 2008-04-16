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

  File:		YEvent.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui-events"
#include "YUILog.h"

#include "YEvent.h"
#include "YSimpleEventHandler.h"

using std::string;


#define VERBOSE_EVENTS	0
#define VERBOSE_BLOCK	0


YSimpleEventHandler::YSimpleEventHandler()
{
    _pendingEvent	= 0;
    _eventsBlocked	= false;
}


YSimpleEventHandler::~YSimpleEventHandler()
{
    clear();
}


void YSimpleEventHandler::clear()
{
    if ( _pendingEvent )
    {
#if VERBOSE_EVENTS
	yuiDebug() << "Clearing pending event: " << YEvent::toString( _pendingEvent->eventType() ) << endl;
#endif
	deleteEvent( _pendingEvent );
    }
}


YEvent * YSimpleEventHandler::consumePendingEvent()
{
    YEvent * event = _pendingEvent;
    _pendingEvent = 0;

    return event;
}


void YSimpleEventHandler::sendEvent( YEvent * event )
{
    if ( ! event )
    {
	yuiError() << "Ignoring NULL event" << endl;
	return;
    }

    if ( eventsBlocked() )
    {
#if VERBOSE_BLOCK
	yuiDebug() << "Blocking " << YEvent::toString( event->eventType() ) << " event" << endl;
#endif
	// Avoid memory leak: The event handler assumes ownership of the newly
	// created event, so we have to clean it up here.
	deleteEvent( event );

	return;
    }

    if ( _pendingEvent )
    {
	/**
	 * This simple event handler keeps track of only the latest user event.
	 * If there is more than one, older events are automatically discarded.
	 * Since Events are created on the heap with the "new" operator,
	 * discarded events need to be deleted.
	 *
	 * Events that are not discarded are deleted later (after they are
	 * processed) by the generic UI.
	 **/

	deleteEvent( _pendingEvent );
    }

#if VERBOSE_EVENTS
    yuiDebug() << "New pending event: " << YEvent::toString( event->eventType() ) << endl;
#endif

    _pendingEvent = event;
}


bool
YSimpleEventHandler::eventPendingFor( YWidget * widget ) const
{
    YWidgetEvent * event = dynamic_cast<YWidgetEvent *> (_pendingEvent);

    if ( ! event )
	return false;

    return event->widget() == widget;
}


void YSimpleEventHandler::blockEvents( bool block )
{
#if VERBOSE_BLOCK
    if ( block )	yuiDebug() << "Blocking events"   << endl;
    else		yuiDebug() << "Unblocking events" << endl;
#endif

    _eventsBlocked = block;
}


void YSimpleEventHandler::deleteEvent( YEvent * event )
{
    if ( event == _pendingEvent )
	_pendingEvent = 0;
    
    if ( event )
	delete event;
}

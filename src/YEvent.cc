/*
  |****************************************************************************
  |
  | Copyright (c) 2000 - 2012 Novell, Inc.
  | All Rights Reserved.
  |
  | This program is free software; you can redistribute it and/or
  | modify it under the terms of version 2 of the GNU General Public License as
  | published by the Free Software Foundation.
  |
  | This program is distributed in the hope that it will be useful,
  | but WITHOUT ANY WARRANTY; without even the implied warranty of
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
  | GNU General Public License for more details.
  |
  | You should have received a copy of the GNU General Public License
  | along with this program; if not, contact Novell, Inc.
  |
  | To contact Novell about this file by physical or electronic mail,
  | you may find current contact information at www.novell.com
  |
  |****************************************************************************
*/


/*-/

  File:		YEvent.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/



#define YUILogComponent "ui-events"
#include "YUILog.h"

#include "YWidget.h"
#include "YEvent.h"
#include "YDialog.h"


unsigned long YEvent::_nextSerial = 0;


YEvent::YEvent( EventType eventType )
    : _eventType( eventType )
{
    _dialog = YDialog::currentDialog( false ); // don't throw
    _serial = _nextSerial++;
}


YEvent::~YEvent()
{
    invalidate();
}


bool
YEvent::isValid() const
{
    return _eventType != InvalidEvent;
}


void
YEvent::invalidate()
{
    _eventType = InvalidEvent;
}


const char *
YEvent::toString( EventType eventType )
{
    switch ( eventType )
    {
	case NoEvent:			return "NoEvent";
	case UnknownEvent:		return "UnknownEvent";
	case WidgetEvent:		return "WidgetEvent";
	case MenuEvent:			return "MenuEvent";
	case KeyEvent:			return "KeyEvent";
	case CancelEvent:		return "CancelEvent";
	case TimeoutEvent:		return "TimeoutEvent";
	case DebugEvent:		return "DebugEvent";
	case InvalidEvent:		return "InvalidEvent";

	// Intentionally omitting "default" branch so the compiler can
	// detect unhandled enums
    }

    return "<Unknown event type - internal error>";
}


const char *
YEvent::toString( EventReason reason )
{
    switch ( reason )
    {
	case UnknownReason:		return "Unknown";
	case Activated:			return "Activated";
	case SelectionChanged:		return "SelectionChanged";
	case ValueChanged:		return "ValueChanged";
	case ContextMenuActivated:	return "ContextMenuActivated";

	// Intentionally omitting "default" branch so the compiler can
	// detect unhandled enums
    }

    return "<Unknown event reason - internal error>";
}




YWidgetEvent::YWidgetEvent( YWidget *	widget,
			    EventReason	reason,
			    EventType	eventType )
    : YEvent( eventType )
    , _widget( widget )
    , _reason( reason )
{
    if ( widget )
	setDialog( widget->findDialog() );
}



YKeyEvent::YKeyEvent( const std::string &	keySymbol,
		      YWidget *			focusWidget )
    : YEvent( KeyEvent )
    , _keySymbol( keySymbol )
    , _focusWidget( focusWidget )
{
}



std::ostream &
operator<<( std::ostream & stream, const YEvent * event )
{
    if ( event )
    {
	stream << YEvent::toString( event->eventType() )
	       << " at " << std::hex << (void *) event << std::dec;
    }
    else
    {
	stream << "<NULL event>";
    }

    return stream;
}

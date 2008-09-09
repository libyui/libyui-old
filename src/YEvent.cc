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

#include "YWidget.h"
#include "YEvent.h"
#include "YDialog.h"

using std::string;

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



YKeyEvent::YKeyEvent( const string &	keySymbol,
		      YWidget *		focusWidget )
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
	       << " at " << hex << (void *) event << dec;
    }
    else
    {
	stream << "<NULL event>";
    }

    return stream;
}

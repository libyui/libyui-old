/*************************************************************************************************************

 Copyright (C) 2000 - 2010 Novell, Inc.   All Rights Reserved.

 This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
 Public License as published by the Free Software Foundation; either version 2 of the License, or (at your
 option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License along with this program; if not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*************************************************************************************************************/



 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////   __/\\\\\\_____________/\\\__________/\\\________/\\\___/\\\________/\\\___/\\\\\\\\\\\_           ////
 ////    _\////\\\____________\/\\\_________\///\\\____/\\\/___\/\\\_______\/\\\__\/////\\\///__          ////
 ////     ____\/\\\______/\\\__\/\\\___________\///\\\/\\\/_____\/\\\_______\/\\\______\/\\\_____         ////
 ////      ____\/\\\_____\///___\/\\\_____________\///\\\/_______\/\\\_______\/\\\______\/\\\_____        ////
 ////       ____\/\\\______/\\\__\/\\\\\\\\\_________\/\\\________\/\\\_______\/\\\______\/\\\_____       ////
 ////        ____\/\\\_____\/\\\__\/\\\////\\\________\/\\\________\/\\\_______\/\\\______\/\\\_____      ////
 ////         ____\/\\\_____\/\\\__\/\\\__\/\\\________\/\\\________\//\\\______/\\\_______\/\\\_____     ////
 ////          __/\\\\\\\\\__\/\\\__\/\\\\\\\\\_________\/\\\_________\///\\\\\\\\\/_____/\\\\\\\\\\\_    ////
 ////           _\/////////___\///___\/////////__________\///____________\/////////______\///////////__   ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                 widget abstraction library providing Qt, GTK and ncurses frontends                  ////
 ////                                                                                                     ////
 ////                                   3 UIs for the price of one code                                   ////
 ////                                                                                                     ////
 ////                                       ***  core library  ***                                        ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                              (C) SUSE Linux GmbH    ////
 ////                                                                                                     ////
 ////                                                              libYUI-AsciiArt (C) 2012 Björn Esser   ////
 ////                                                                                                     ////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

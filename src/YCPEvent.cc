/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/


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

  File:		YCPEvent.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <ycp/YCPVoid.h>
#include <ycp/YCPString.h>
#include <ycp/YCPInteger.h>
#include <ycp/YCPSymbol.h>

#include "YCPEvent.h"
#include <yui/YWidget.h>
#include "YCPItem.h"
#include "YCPMenuItem.h"
#include "YCPValueWidgetID.h"

#define YUILogComponent "ui"
#include <yui/YUILog.h>




YCPEvent::YCPEvent( const YEvent * event )
    : _event( event )
{
}



YCPValue
YCPEvent::eventId() const
{
    if ( ! _event )
	return YCPVoid();
    
    //
    // Widget Events
    //

    const YWidgetEvent * widgetEvent = dynamic_cast<const YWidgetEvent *> (_event);

    if ( widgetEvent )
    {
	if ( widgetEvent->widget() && widgetEvent->widget()->hasId() )
	{
	    YCPValueWidgetID * id = dynamic_cast<YCPValueWidgetID *> ( widgetEvent->widget()->id() );

	    if ( id )
		return id->value();
	}
    }


    //
    // Menu Events
    //

    const YMenuEvent * menuEvent = dynamic_cast<const YMenuEvent *> (_event);

    if ( menuEvent )
    {
	if ( menuEvent->item() )
	{
	    YCPMenuItem * ycpMenuItem = dynamic_cast<YCPMenuItem *> ( menuEvent->item() );

	    if ( ycpMenuItem )
		return ycpMenuItem->id();

	    YCPItem * ycpItem = dynamic_cast<YCPItem *> ( menuEvent->item() );

	    if ( ycpItem )
		return ycpItem->id();

	    return YCPString( menuEvent->item()->label() );
	}
	else
	{
	    return YCPString( menuEvent->id() );
	}
    }


    //
    // Key Events
    //

    const YKeyEvent * keyEvent = dynamic_cast<const YKeyEvent *> (_event);

    if ( keyEvent )
    {
	return YCPString( keyEvent->keySymbol() );
    }

    
    //
    // Misc Simple Events
    //

    if ( dynamic_cast<const YCancelEvent  *> (_event ) )  return YCPSymbol( "cancel"  );
    if ( dynamic_cast<const YTimeoutEvent *> (_event ) )  return YCPSymbol( "timeout" );
    if ( dynamic_cast<const YDebugEvent   *> (_event ) )  return YCPSymbol( "debugHotkey" );

    return YCPVoid();
}



YCPMap
YCPEvent::eventMap() const
{
    YCPMap map;

    if ( _event )
    {
	map->add( YCPString( "EventType"     ), YCPString ( YEvent::toString( _event->eventType() ) ) );
	map->add( YCPString( "EventSerialNo" ), YCPInteger( _event->serial() ) );

	YCPValue id = eventId();

	if ( ! id.isNull() && ! id->isVoid() )
	{
	    map->add( YCPString( "ID" ), id );
	}

	addWidgetEventFields( map, id );
	addKeyEventFields   ( map, id );
    }

    return map;
}



void
YCPEvent::addWidgetEventFields( YCPMap & map, const YCPValue & id ) const
{
    const YWidgetEvent * widgetEvent = dynamic_cast<const YWidgetEvent *> (_event);

    if ( ! widgetEvent )
	return;

    map->add( YCPString( "EventReason" ), YCPString ( YEvent::toString( widgetEvent->reason() )	) );

    YWidget * widget = widgetEvent->widget();

    if ( widget )
    {
	// Add widget specific info:
	// Add WidgetID

	if ( ! id.isNull() && ! id->isVoid() )
	{
	    map->add( YCPString( "WidgetID" ), id );	// Just an alias for "ID"
	}


	// Add WidgetClass

	const char * widgetClass = widget->widgetClass();

	if ( widgetClass )
	{
	    if ( *widgetClass == 'Y' )	// skip leading "Y" (YPushButton, YInputField, ...)
		widgetClass++;

	    map->add( YCPString( "WidgetClass" ), YCPSymbol( widgetClass ) );
	}


	// Add the Widget's debug label.
	// This is usually the label (translated to the user's locale).

	string debugLabel = widget->debugLabel();

	if ( ! debugLabel.empty() )
	    map->add( YCPString( "WidgetDebugLabel" ), YCPString( debugLabel ) );
    }
}



void
YCPEvent::addKeyEventFields( YCPMap & map, const YCPValue & id ) const
{
    const YKeyEvent * keyEvent = dynamic_cast<const YKeyEvent *> (_event);

    if ( ! keyEvent )
	return;

    if ( ! id.isNull() && ! id->isVoid() )
    {
	map->add( YCPString( "KeySymbol" ), id ); // Just an alias for "ID"
    }

    YWidget * focusWidget = keyEvent->focusWidget();

    if ( focusWidget )
    {
	// Add widget specific info:
	// Add ID of the focus widget

	if ( focusWidget->hasId() )
	{
	    YCPValueWidgetID * id = dynamic_cast<YCPValueWidgetID *> ( focusWidget->id() );

	    if ( id )
		map->add( YCPString( "FocusWidgetID" ), id->value() ); // just an alias
	}


	// Add WidgetClass

	const char * widgetClass = focusWidget->widgetClass();

	if ( widgetClass )
	{
	    if ( *widgetClass == 'Y' )	// skip leading "Y" (YPushButton, YInputField, ...)
		widgetClass++;

	    map->add( YCPString( "FocusWidgetClass" ), YCPSymbol( widgetClass ) );
	}


	// Add the Widget's shortcut property.
	// This is usually the label (translated to the user's locale).

	string debugLabel = focusWidget->debugLabel();

	if ( ! debugLabel.empty() )
	    map->add( YCPString( "FocusWidgetDebugLabel" ), YCPString( debugLabel ) );
    }
}



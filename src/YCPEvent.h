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

  File:		YCPEvent.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPEvent_h
#define YCPEvent_h


#include <ycp/YCPValue.h>
#include <ycp/YCPMap.h>

#include <yui/YEvent.h>


/**
 * Adaptor class to translate YEvents to YCPValues.
 **/
class YCPEvent
{
public:

    /**
     * Constructor.
     *
     * This does NOT transfer ownership of 'event' to this object.
     **/
    YCPEvent( const YEvent * event );

    /**
     * Return the event ID for this event.
     * This is the same that UI::UserInput() returns.
     **/
    YCPValue eventId() const;

    /**
     * Return the event map for this event.
     * This is the same map that UI::WaitForEvent() returns.
     **/
    YCPMap eventMap() const;

    /**
     * Return the raw event.
     **/
    const YEvent * event() const { return _event; }
    
private:

    /**
     * Add the map fields (key/value pairs) for a widget event
     * if this is a widget event. Do nothing otherwise.
     **/
    void addWidgetEventFields( YCPMap & map, const YCPValue & id ) const;
    
    /**
     * Add the map fields (key/value pairs) for a key event
     * if this is a key event. Do nothing otherwise.
     **/
    void addKeyEventFields   ( YCPMap & map, const YCPValue & id ) const;
    

    const YEvent * _event;
};



#endif // YCPEvent_h

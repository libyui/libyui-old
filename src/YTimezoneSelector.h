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

  File:		YTimezoneSelector.h

  Author:	Stephan Kulow <coolo@suse.de>

/-*/

#ifndef YTimezoneSelector_h
#define YTimezoneSelector_h

#include <map>
#include "YWidget.h"

using std::map;

class YTimezoneSelectorPrivate;


class YTimezoneSelector : public YWidget
{
protected:
    /**
     * Constructor. This widget isn't doing much on it's own, but the
     * UI may have some fancy use. @arg pixmap should be a png or jpg
     * of a world map with centered 0°0° and the timezones are a map
     * between zone.tab entry and user visible string.
     *
     * The widget is only displaying timezones/cities in that map
     **/
    YTimezoneSelector( YWidget *parent,
                       const string &pixmap,
		       const map<string, string> &timezones );

public:
    /**
     * Destructor.
     **/
    virtual ~YTimezoneSelector();

    /**
     * Return a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YTimezoneSelector"; }

    /**
     * Set a property.
     * Reimplemented from YWidget.
     *
     * This function may throw YUIPropertyExceptions.
     *
     * This function returns 'true' if the value was successfully set and
     * 'false' if that value requires special handling (not in error cases:
     * those are covered by exceptions).
     **/
    virtual bool setProperty( const string & propertyName,
			      const YPropertyValue & val );

    /**
     * Get a property.
     * Reimplemented from YWidget.
     *
     * This method may throw YUIPropertyExceptions.
     **/
    virtual YPropertyValue getProperty( const string & propertyName );

    /**
     * Return this class's property set.
     * This also initializes the property upon the first call.
     *
     * Reimplemented from YWidget.
     **/
    virtual const YPropertySet & propertySet();

    /**
     * subclasses have to implement this to return value
     */
    virtual std::string currentZone() const = 0;

    /**
     * subclasses have to implement this to set value
     */
    virtual void setCurrentZone( const std::string &zone, bool zoom ) = 0;

private:
    ImplPtr<YTimezoneSelectorPrivate> priv;

};


#endif // YMultiProgressMeter_h

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

  File:		YMacroRecorder.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YMacroRecorder_h
#define YMacroRecorder_h

#include <string>

using std::string;
class YWidget;


/**
 * Abstract base class for macro recorders.
 *
 * Applications should not use this directly, but the static methods in YMacro.
 **/
class YMacroRecorder
{
    friend class YMacro;

protected:

    /**
     * Constructor
     **/
    YMacroRecorder() {}

public:
    /**
     * Destructor
     **/
    virtual ~YMacroRecorder() {}

    /**
     * Start recording a macro to the specified file.
     **/
    virtual void record( const string & macroFileName ) = 0;

    /**
     * End recording and close the current macro file (if there is any).
     **/
    virtual void endRecording() = 0;

    /**
     * Return 'true' if a macro is currently being recorded.
     **/
    virtual bool recording() const = 0;

    /**
     * Record one widget property.
     **/
    virtual void recordWidgetProperty( YWidget *	widget,
				       const char *	propertyName ) = 0;

    /**
     * Record a "UI::MakeScreenShot()" statement.
     *
     * If 'enabled' is 'false', this statement will be commented out.
     * If no file name is given, a default file name (with auto-increment) will
     * be used.
     **/
    virtual void recordMakeScreenShot( bool enabled = false,
				       const string & filename = string() ) = 0;
};

#endif // YMacroRecorder_h

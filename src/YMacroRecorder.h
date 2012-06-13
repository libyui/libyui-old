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

  File:		YMacroRecorder.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YMacroRecorder_h
#define YMacroRecorder_h

#include <string>

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
    virtual void record( const std::string & macroFileName ) = 0;

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
				       const std::string & filename = std::string() ) = 0;
};

#endif // YMacroRecorder_h

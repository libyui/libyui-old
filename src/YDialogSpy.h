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

  File:		YDialogSpy.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YDialogSpy_h
#define YDialogSpy_h

#include "ImplPtr.h"

class YWidget;
class YDialog;
class YDialogSpyPrivate;


/**
 * An interactive dialog debugger: Show the structure and content of a dialog
 * and its widgets.
 *
 * This can be invoked by special key combinations:
 * Ctrl-Alt-Shift-Y in the Qt UI
 **/
class YDialogSpy
{
public:
    /**
     * Show a YDialogSpy for the specified dialog. 0 means "use the topmost
     * dialog".
     * This will return only when the user closes the YDialogSpy dialog.
     **/
    static void showDialogSpy( YDialog * dialog = 0 );

    /**
     * Show the "Properties" sub-window.
     **/
    void showProperties();

    /**
     * Hide the "Properties" sub-window.
     **/
    void hideProperties();

    /**
     * Return 'true' if the "Properties" sub-window is currently shown,
     * 'false' if not.
     **/
    bool propertiesShown() const;

protected:
    /**
     * Constructor: Create a YDialogSpy for the specified dialog. 0 means "use
     * the topmost dialog".
     *
     * In most cases it is more useful to use the static showDialogSpy() method
     * rather than create this dialog directly.
     **/
    YDialogSpy( YDialog * dialog = 0 );

    /**
     * Destructor.
     **/
    virtual ~YDialogSpy();

    /**
     * Execute the event loop. This will only return when the user closes the
     * YDialogSpy dialog.
     **/
    void exec();

    /**
     * Show the properties of the specified widget if the "Properties"
     * sub-window is currently shown.
     **/
    void showProperties( YWidget * widget );

private:

    ImplPtr<YDialogSpyPrivate> priv;
};


#endif // YDialogSpy_h

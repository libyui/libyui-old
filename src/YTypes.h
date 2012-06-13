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

  File:		YTypes.h

  Author:	Stefan Hundhammer <sh@suse.de>

  Header file for frequently used simple types to reduce interdependencies
  between important headers (e.g., YWidget.h, YUI.h).

/-*/


#ifndef YTypes_h
#define YTypes_h

#include <list>

typedef double		YLayoutSize_t;
typedef long long	YFileSize_t;

class YWidget;

typedef std::list<YWidget *>				YWidgetList;
typedef std::list<YWidget *>::iterator			YWidgetListIterator;
typedef std::list<YWidget *>::const_iterator		YWidgetListConstIterator;
typedef std::list<YWidget *>::reverse_iterator		YWidgetListReverseIterator;
typedef std::list<YWidget *>::const_reverse_iterator	YWidgetListConstReverseIterator;


#define YUIAllDimensions	2

enum YUIDimension
{
    YD_HORIZ	= 0,
    YD_VERT	= 1
};


enum YAlignmentType
{
    YAlignUnchanged,
    YAlignBegin,
    YAlignEnd,
    YAlignCenter
};


enum YDialogType
{
    YMainDialog,
    YPopupDialog,
    YWizardDialog,
};


enum YDialogColorMode
{
    YDialogNormalColor,	// Default
    YDialogInfoColor,	// Brighter colors
    YDialogWarnColor	// Very bright Warning colors
};


enum YButtonRole
{
    YCustomButton = 0,	// No predefined role
    YOKButton,		// [OK], [Continue], [Yes], [Accept], [Next]
    YApplyButton,	// [Apply]
    YCancelButton,	// [Cancel]
    YHelpButton,	// [Help]

    YMaxButtonRole	// For use as array size
};


enum YButtonOrder
{
    YKDEButtonOrder,	// [OK] [Apply] [Cancel] [Custom1] [Custom2] [Help]
			// [Yes] [No]
			// [Continue] [Cancel]

    YGnomeButtonOrder	// [Help] [Custom1] [Custom2] [Apply] [Cancel] [OK]
			// [No] [Yes]
			// [Cancel] [Continue]
};



#endif // YTypes_h

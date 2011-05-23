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

  File:		YReplacePoint.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YReplacePoint_h
#define YReplacePoint_h

#include "YSingleChildContainerWidget.h"

class YReplacePoint : public YSingleChildContainerWidget
{
protected:
    /**
     * Constructor
     **/
    YReplacePoint( YWidget * parent );

public:
    /**
     * Show a newly added child. The application using the ReplacePoint is
     * required to call this after the new child is created.
     *
     * This cannot be done in the child widget's constructor (e.g., by
     * overwriting YWidget::addChild()) since at that point
     * YWidget::widgetRep() may or may not be initialized yet.
     *
     * This default implementation does nothing. Derived classes should
     * reimplement this to make new child widgets visible.
     **/
    virtual void showChild();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YReplacePoint"; }
};


#endif // YReplacePoint_h


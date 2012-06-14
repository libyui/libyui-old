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


/*-/

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

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

  File:		YEmpty.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YEmpty_h
#define YEmpty_h

#include "YWidget.h"
#include "ImplPtr.h"


class YEmptyPrivate;


class YEmpty : public YWidget
{
protected:
    /**
     * Constructor.
     **/
    YEmpty( YWidget * parent );

public:
    /**
     * Destructor.
     **/
    virtual ~YEmpty();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YEmpty"; }

    /**
     * Preferred width of the widget.
     *
     * Reimplemented from YWidget.
     **/
    virtual int preferredWidth();

    /**
     * Preferred height of the widget.
     *
     * Reimplemented from YWidget.
     **/
    virtual int preferredHeight();

private:

    ImplPtr<YEmptyPrivate> priv;
};


#endif // YEmpty_h

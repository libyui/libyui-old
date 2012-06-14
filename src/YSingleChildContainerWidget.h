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

  File:		YSingleChildContainerWidget.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YSingleChildContainerWidget_h
#define YSingleChildContainerWidget_h

#include "YWidget.h"


/**
 * Container widget class that manages one child.
 **/
class YSingleChildContainerWidget: public YWidget
{
protected:
    /**
     * Constructor.
     **/
    YSingleChildContainerWidget( YWidget * parent );

public:
    /**
     * Destructor.
     **/
    virtual ~YSingleChildContainerWidget();

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

    /**
     * Set the new size of the widget.
     * In this case, the size of the single child is set.
     *
     * Reimplemented from YWidget.
     **/
    virtual void setSize( int newWidth, int newHeight );

    /**
     * Returns 'true' if this widget is stretchable in the specified dimension.
     * In this case, the stretchability of the single child is returned.
     *
     * Reimplemented from YWidget.
     **/
    virtual bool stretchable( YUIDimension dim ) const;
};



#endif // YSingleChildContainerWidget_h

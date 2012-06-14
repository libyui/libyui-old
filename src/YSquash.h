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

  File:		YSquash.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YSquash_h
#define YSquash_h

#include "YSingleChildContainerWidget.h"
#include "ImplPtr.h"


class YSquashPrivate;

/**
 * HSquash, VSquash HVSquash:
 *
 * Squash is a widget that "squashes" its one child during layout, i.e., it
 * reduces it in size down to its preferred size. It may squash vertically,
 * horizontally or in both dimensions.
 **/
class YSquash : public YSingleChildContainerWidget
{
protected:
    /**
     * Constructor.
     *
     * Squashes horizontally if 'horSquash' is 'true',
     * vertically if 'vertSquash' is 'true'.
     **/
    YSquash( YWidget * parent, bool horSquash, bool vertSquash );

public:
    /**
     * Destructor.
     **/
    virtual ~YSquash();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const;

    /**
     * Returns 'true' if this widget squashes horizontally.
     **/
    bool horSquash() const;

    /**
     * Returns 'true' if this widget squashes vertically.
     **/
    bool vertSquash() const;


    /**
     * In a squashed dimension the widget NOT stretchable.
     * In an unsquashed dimension the widget is stretchable if the
     * child is stretchable.
     **/
    bool stretchable( YUIDimension dim ) const;

private:

    ImplPtr<YSquashPrivate> priv;
};


#endif // YSquash_h

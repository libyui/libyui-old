/*
  Copyright (c) [2019] SUSE LLC
  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) version 3.0 of the License. This library
  is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
  License for more details. You should have received a copy of the GNU
  Lesser General Public License along with this library; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301 USA
*/


/*-/

  File:		YItemSelector.h

  Author:	Stefan Hundhammer <shundhammer@suse.de>

/-*/

#ifndef YItemSelector_h
#define YItemSelector_h

#include "YSelectionWidget.h"
#include "YDescribedItem.h"


class YItemSelectorPrivate;

/**
 * Scrollable item selector widget with not only a label for each item, but
 * also a (possible multi-line) description and an optional icon.
 *
 * This widget supports both 1-of-n or n-of-m selection, i.e. it can act as a
 * (more verbose and more screen space consuming) replacement for YSelectionBox
 * or YMultiSelectionBox.
 **/
class YItemSelector: public YSelectionWidget
{
public:

    /**
     * Constructor.
     **/
    YItemSelector( YWidget *    parent,
                   bool         enforceSingleSelection = true );

    /**
     * Destructor.
     **/
    virtual ~YItemSelector();


private:

    ImplPtr<YItemSelectorPrivate> priv;

};      // class YItemSelector

#endif  // YItemSelector_h

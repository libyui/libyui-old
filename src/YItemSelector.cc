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

  File:		YItemSelector.cc

  Author:	Stefan Hundhammer <shundhammer@suse.de>

/-*/

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YItemSelector.h"

struct YItemSelectorPrivate
{
    bool dummy;
};


YItemSelector::YItemSelector( YWidget *    parent,
                              bool         enforceSingleSelection )
    : YSelectionWidget( parent,
                        "",     // label
                        enforceSingleSelection )
    , priv( new YItemSelectorPrivate )
{
    
}


YItemSelector::~YItemSelector()
{
    YUI_CHECK_NEW( priv );
    
    setDefaultStretchable( YD_HORIZ, true );
    setDefaultStretchable( YD_VERT,  true );
}


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

  File:		YSlider.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui"
#include "YUILog.h"

#include "YSlider.h"


struct YSliderPrivate
{
    YSliderPrivate()
	{}

    bool dummy;
};




YSlider::YSlider( YWidget *		parent,
		  const std::string & 	label,
		  int			minValue,
		  int			maxValue )
    : YIntField( parent, label, minValue, maxValue )
    , priv( new YSliderPrivate() )
{
    YUI_CHECK_NEW( priv );

    setDefaultStretchable( YD_HORIZ, true );
    setStretchable( YD_VERT, false );
}


YSlider::~YSlider()
{
    // NOP
}


//
// Property documentation - all inherited from YIntField:
//

/*
 * @property integer		Value		the slider value
 * @property integer		MinValue	the minimum value
 * @property integer		MaxValue	the maximum value
 * @property std::string	Label		caption above the slider
 */

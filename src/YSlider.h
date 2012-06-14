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

  File:		YSlider.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YSlider_h
#define YSlider_h

#include "YIntField.h"

class YSliderPrivate;


/**
 * Slider: Input widget for an integer value between a minimum and a maximum
 * value. Very similar to IntField in semantics, but with a graphical slider
 * that can be dragged to the desired value. It also contains an IntField to
 * allow entering the value directly.
 *
 * Don't confuse this widget with ProgressBar: ProgressBar is output-only.
 *
 *
 * This is an optional widget, i.e. not all UIs support it.
 **/
class YSlider : public YIntField
{
protected:
    /**
     * Constructor.
     *
     * Create a Slider with 'label' as the caption, and the specified minimum
     * and maximum values.
     *
     * Note that YWidgetFactory::createSlider() also has an 'initialValue'
     * parameter that is not used here (because the current value is not stored
     * in this base class, but in the derived class).
     **/
    YSlider( YWidget *			parent,
	     const std::string &	label,
	     int			minValue,
	     int			maxValue );

public:
    /**
     * Destructor.
     **/
    virtual ~YSlider();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YSlider"; }

private:

    ImplPtr<YSliderPrivate> priv;
};


#endif // YSlider_h

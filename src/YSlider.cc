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
		  const string & 	label,
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
 * @property integer Value		the slider value
 * @property integer MinValue		the minimum value
 * @property integer MaxValue		the maximum value
 * @property string  Label		caption above the slider
 */


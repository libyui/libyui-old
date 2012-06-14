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

  File:		YEmpty.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include "YSpacing.h"
#include "YUI.h"
#include "YApplication.h"


struct YSpacingPrivate
{
    YSpacingPrivate( YUIDimension dim, int size )
	: dim( dim )
	, size( size )
	{}

    YUIDimension	dim;
    int			size;
};




YSpacing::YSpacing( YWidget * parent, YUIDimension dim, bool stretchable, YLayoutSize_t layoutUnits )
    : YWidget( parent )
    , priv( new YSpacingPrivate( dim, YUI::app()->deviceUnits( dim, layoutUnits ) ) )
{
    YUI_CHECK_NEW( priv );
    setStretchable( dim, stretchable );
    setStretchable( dim == YD_HORIZ ? YD_VERT : YD_HORIZ, false );
}


YSpacing::~YSpacing()
{
    // NOP
}


YUIDimension
YSpacing::dimension() const
{
    return priv->dim;
}


int YSpacing::size() const
{
    return priv->size;
}


int YSpacing::size( YUIDimension dim ) const
{
    if ( dim == priv->dim )	return priv->size;
    else			return 0;
}


int YSpacing::preferredWidth()
{
    if ( priv->dim == YD_HORIZ )
	return priv->size;
    else
	return 0;
}


int YSpacing::preferredHeight()
{
    if ( priv->dim == YD_VERT )
	return priv->size;
    else
	return 0;
}


const char *
YSpacing::widgetClass() const
{
    if ( priv->size > 0 )
	return ( priv->dim == YD_HORIZ ) ? "YHSpacing" : "YVSpacing";

    if ( stretchable( YD_HORIZ ) )	return "YHStretch";
    if ( stretchable( YD_VERT  ) )	return "YVStretch";

    return "YSpacing";
}

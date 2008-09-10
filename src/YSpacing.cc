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

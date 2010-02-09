/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/


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

  File:		YImage.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include "YImage.h"
#include "YBothDim.h"


struct YImagePrivate
{
    /**
     * Constructor.
     **/
    YImagePrivate( const string & imageFileName, bool animated )
	: imageFileName( imageFileName )
	, animated( animated )
	, autoScale( false )
    {
	zeroSize.hor  = false;
	zeroSize.vert = false;
    }

    
    string		imageFileName;
    bool		animated;
    YBothDim<bool>	zeroSize;
    bool		autoScale;
};




YImage::YImage( YWidget *	parent,
		const string &	imageFileName,
		bool 		animated )
    : YWidget( parent )
    , priv( new YImagePrivate( imageFileName, animated ) )
{
    YUI_CHECK_NEW( priv );
}


YImage::~YImage()
{
    // NOP
}


string YImage::imageFileName() const
{
    return priv->imageFileName;
}


bool YImage::animated() const
{
    return priv->animated;
}


void YImage::setImage( const string & imageFileName, bool animated )
{
    priv->imageFileName = imageFileName;
    priv->animated	= animated;
}


bool YImage::hasZeroSize( YUIDimension dim ) const
{
    return priv->zeroSize[ dim ];
}


void YImage::setZeroSize( YUIDimension dim, bool zeroSize )
{
    priv->zeroSize[ dim ] = zeroSize;
    setStretchable( dim, zeroSize );
}


bool YImage::autoScale() const
{
    return priv->autoScale;
}


void YImage::setAutoScale( bool autoScale )
{
    priv->autoScale = autoScale;
}




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




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

   File:	YSquash.cc

   Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#include "YSquash.h"
#include "YBothDim.h"


struct YSquashPrivate
{
    /**
     * Constructor.
     **/ 
    YSquashPrivate( bool horSquash, bool vertSquash )
    {
	squash.hor  = horSquash;
	squash.vert = vertSquash;
    }
    
    YBothDim<bool> squash;
};
    

YSquash::YSquash( YWidget * parent, bool horSquash, bool vertSquash )
    : YSingleChildContainerWidget( parent )
    , priv( new YSquashPrivate( horSquash, vertSquash ) )
{
    YUI_CHECK_NEW( priv );
}


YSquash::~YSquash()
{
    // NOP
}


bool YSquash::horSquash() const
{
    return priv->squash.hor;
}


bool YSquash::vertSquash() const
{
    return priv->squash.vert;
}


bool YSquash::stretchable( YUIDimension dim ) const
{
    if ( ! hasChildren() )
	return false;
    
    return ! priv->squash[ dim ] && firstChild()->stretchable( dim );
}


const char *
YSquash::widgetClass() const
{
    if      ( priv->squash.hor && priv->squash.vert )	return "YHVSquash";
    else if ( priv->squash.hor  )			return "YHSquash";
    else if ( priv->squash.vert )			return "YVSquash";
    else						return "YSquash_NoSquash";
}

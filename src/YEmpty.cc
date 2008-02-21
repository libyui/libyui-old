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


#include "YEmpty.h"

struct YEmptyPrivate
{
    bool dummy;
};




YEmpty::YEmpty( YWidget * parent )
    :YWidget( parent )
    , priv( new YEmptyPrivate )
{
    YUI_CHECK_NEW( priv );
}


YEmpty::~YEmpty()
{
    // NOP
}


int YEmpty::preferredWidth()
{
    return 0;
}


int YEmpty::preferredHeight()
{
    return 0;
}


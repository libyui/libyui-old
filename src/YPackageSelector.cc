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

  File:		YPackageSelector.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define YUILogComponent "ui-pkg"
#include "YUILog.h"

#include "YPackageSelector.h"


YPackageSelector::YPackageSelector( YWidget * parent, long modeFlags )
    : YWidget( parent )
    , _modeFlags( modeFlags )
{
    yuiMilestone() << "YPackageSelector flags: " << hex << modeFlags << dec << endl;

    setDefaultStretchable( YD_HORIZ, true );
    setDefaultStretchable( YD_VERT,  true );
}



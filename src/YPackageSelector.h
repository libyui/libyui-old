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

  File:		YPackageSelector.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YPackageSelector_h
#define YPackageSelector_h

#include "YWidget.h"

#define YPkg_TestMode		1 << 0
#define YPkg_OnlineUpdateMode	1 << 1
#define YPkg_UpdateMode		1 << 2
#define YPkg_SearchMode		1 << 3
#define YPkg_SummaryMode	1 << 4
#define YPkg_RepoMode		1 << 5

#define YPkg_RepoMgr		1 << 15


class YPackageSelector : public YWidget
{
protected:
    /**
     * Constructor.
     *
     * 'modeFlags' are flags determining which modes to use, ORed together:
     *     YPkg_OnlineUpdateMode | YPkg_TestMode
     **/
    YPackageSelector( YWidget * parent, long modeFlags = 0 );

public:
    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YPackageSelector"; }

    /**
     * Check for the various modes.
     **/
    bool testMode()		const { return _modeFlags & YPkg_TestMode;	   }
    bool onlineUpdateMode()	const { return _modeFlags & YPkg_OnlineUpdateMode; }
    bool updateMode()		const { return _modeFlags & YPkg_UpdateMode;	   }
    bool searchMode() 		const { return _modeFlags & YPkg_SearchMode;	   }
    bool summaryMode()		const { return _modeFlags & YPkg_SummaryMode;	   }
    bool repoMode()		const { return _modeFlags & YPkg_RepoMode;         }

    bool repoMgrEnabled()	const { return _modeFlags & YPkg_RepoMgr;          }

protected:
    long _modeFlags;
};


#endif // YPackageSelector_h

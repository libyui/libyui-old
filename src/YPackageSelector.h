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

  File:		YPackageSelector.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YPackageSelector_h
#define YPackageSelector_h

#include "YWidget.h"

#define YPkg_TestMode		1 << 0	// Test mode for debugging
#define YPkg_OnlineUpdateMode	1 << 1	// Online update mode: Show patches
#define YPkg_UpdateMode		1 << 2	// Start with "Update problems" filter view
#define YPkg_SearchMode		1 << 3	// Start with "Search"  filter view
#define YPkg_SummaryMode	1 << 4	// Start with "Summary" filter view
#define YPkg_RepoMode		1 << 5	// Start with "Repositories" filter view
#define YPkg_RepoMgr		1 << 6	// Add "Manage Repositories" to menu
#define YPkg_ConfirmUnsupported	1 << 7	// Confirm unsupported packages


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
    bool testMode()		const { return _modeFlags & YPkg_TestMode;	   	}
    bool onlineUpdateMode()	const { return _modeFlags & YPkg_OnlineUpdateMode; 	}
    bool updateMode()		const { return _modeFlags & YPkg_UpdateMode;	   	}
    bool searchMode() 		const { return _modeFlags & YPkg_SearchMode;	   	}
    bool summaryMode()		const { return _modeFlags & YPkg_SummaryMode;	   	}
    bool repoMode()		const { return _modeFlags & YPkg_RepoMode;         	}
    bool repoMgrEnabled()	const { return _modeFlags & YPkg_RepoMgr;		}
    bool confirmUnsupported()	const { return _modeFlags & YPkg_ConfirmUnsupported;	}

protected:
    long _modeFlags;
};


#endif // YPackageSelector_h

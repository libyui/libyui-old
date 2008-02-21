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

  File:		YPackageSelectorPlugin.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YPackageSelectorPlugin_h
#define YPackageSelectorPlugin_h

#include "YUIPlugin.h"

class YWidget;
class YPackageSelector;


/**
 * Abstract base class for simplified access to UI plugins for package selection.
 **/
class YPackageSelectorPlugin: public YUIPlugin
{
protected:
    /**
     * Constructor: Load the specified plugin library
     * from the standard UI plugin directory (/usr/lib/YaST2/plugin).
     **/
    YPackageSelectorPlugin( const char * pluginLibBaseName )
	: YUIPlugin( pluginLibBaseName ) {}

    /**
     * Destructor. Calls dlclose() which will unload the plugin library if it
     * is no longer used, i.e. if the reference count dlopen() uses reaches 0.
     **/
    virtual ~YPackageSelectorPlugin() {}

public:
    /**
     * Create a package selector.
     * Derived classes need to implement this.
     *
     * This might return 0 if the plugin lib could not be loaded or if the
     * appropriate symbol could not be located in the plugin lib. 
     **/
    virtual YPackageSelector * createPackageSelector( YWidget * parent, long modeFlags = 0 ) = 0;
};


#endif // YPackageSelectorPlugin_h

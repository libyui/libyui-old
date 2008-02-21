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

  File:		YUIPlugin.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <dlfcn.h>

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YUIPlugin.h"


#define PLUGIN_PREFIX "libpy2"
#define PLUGIN_SUFFIX ".so.2"


using std::string;


YUIPlugin::YUIPlugin( const char * pluginLibBaseName )
{
    _pluginLibBaseName = string( pluginLibBaseName );

    string pluginFilename = pluginLibFullPath();

    _pluginLibHandle = dlopen( pluginFilename.c_str(),
			       RTLD_NOW | RTLD_GLOBAL);

    if ( ! _pluginLibHandle )
    {
	_errorMsg = dlerror();
	
	yuiError() << "Could not load UI plugin \"" << pluginLibBaseName
		   << "\": " << _errorMsg
		   << endl;
    }
}


YUIPlugin::~YUIPlugin()
{
    // This intentionally does NOT call unload(): This would be
    // counterproductive for almost all use cases of this class. 
}


void
YUIPlugin::unload()
{
    if ( _pluginLibHandle )
	dlclose( _pluginLibHandle );
}


string
YUIPlugin::pluginLibFullPath() const
{
    string fullPath;

    fullPath.append( PLUGINDIR "/" ); // from -DPLUGINDIR in Makefile.am
    fullPath.append( PLUGIN_PREFIX );
    fullPath.append( _pluginLibBaseName );
    fullPath.append( PLUGIN_SUFFIX );

    return fullPath;
}


void * YUIPlugin::locateSymbol( const char * symbol )
{
    if ( ! _pluginLibHandle )
	return 0;
    
    void * addr = dlsym( _pluginLibHandle, symbol );

    if ( ! addr )
    {
	yuiError() << "Could not locate symbol \"" << symbol 
		   << "\" in " << pluginLibFullPath()
		   << endl;
    }

    return addr;
}


bool YUIPlugin::error() const
{
    return _pluginLibHandle == 0;
}


bool YUIPlugin::success() const
{
    return _pluginLibHandle != 0;
}


string YUIPlugin::errorMsg() const
{
    return _errorMsg;
}


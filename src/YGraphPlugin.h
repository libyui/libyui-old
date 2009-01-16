/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|						   (c) SuSE Linux GmbH |
\----------------------------------------------------------------------/

  File:		YGraphPlugin.h

  Author:	Arvin Schnell <aschnell@suse.de>

/-*/

#ifndef YGraphPlugin_h
#define YGraphPlugin_h

#include "YUIPlugin.h"

class YWidget;
class YGraph;


/**
 * Abstract base class for simplified access to UI plugins for graph widget.
 **/
class YGraphPlugin : public YUIPlugin
{
protected:
    /**
     * Constructor: Load the specified plugin library
     * from the standard UI plugin directory (/usr/lib/YaST2/plugin).
     **/
    YGraphPlugin( const char * pluginLibBaseName )
	: YUIPlugin( pluginLibBaseName ) {}

    /**
     * Destructor. Calls dlclose() which will unload the plugin library if it
     * is no longer used, i.e. if the reference count dlopen() uses reaches 0.
     **/
    virtual ~YGraphPlugin() {}

public:
    /**
     * Create a graph widget.
     * Derived classes need to implement this.
     *
     * This might return 0 if the plugin lib could not be loaded or if the
     * appropriate symbol could not be located in the plugin lib. 
     **/
    virtual YGraph * createGraph( YWidget * parent, const string & filename,
				  const string & layoutAlgorithm ) = 0;
};


#endif // YGraphPlugin_h

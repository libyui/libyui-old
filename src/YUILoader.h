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

  File:		YUILoader.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YUILoader_h
#define YUILoader_h


#include <string>

#include "YUI.h"

using std::string;


#define YUIPlugin_Qt		"qt"
#define YUIPlugin_NCurses	"ncurses"
#define YUIPlugin_Gtk		"gtk"


/**
 * Class to load one of the concrete UI plug-ins: Qt, NCurses, Gtk.
 **/
class YUILoader
{
public:
    /**
     * Load any of the available UI plug-ins in this order:
     * - Qt      if $DISPLAY is set
     * - NCurses if stdout is a tty
     **/
    static void loadUI( bool withThreads = false );

    /**
     * Load a UI plug-in. 'name' is one of the YUIPlugin_ -defines above.
     *
     * This might throw exceptions.
     **/
    static void loadPlugin( const string & name, bool withThreads = false );

private:
    YUILoader()  {}
    ~YUILoader() {}
};


/**
 * Every UI plug-in has to provide a function
 *
 *     YUI * createUI( bool withThreads )
 *
 * that creates a UI of that specific type upon the first call and returns that
 * singleton for all subsequent calls.
 **/
typedef YUI * (*createUIFunction_t)( bool );


#endif // YUILoader_h

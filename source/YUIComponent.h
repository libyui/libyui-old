/****************************************************************************

Copyright (c) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, contact Novell, Inc.

To contact Novell about this file by physical or electronic mail,
you may find current contact information at www.novell.com

****************************************************************************


  File:		YUIComponent.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YUIComponent_h
#define YUIComponent_h


#include <Y2.h>
#include <ycp/YCPValue.h>


class YUI;
class Y2Namespace;

/**
 * Abstract base class for YaST2 user interface components.
 * Derive your own component class from this one and implement createUI().
 **/
class YUIComponent : public Y2Component
{
public:
    /**
     * Constructor.
     *
     * 'requestedUIName' can be one of "qt", "ncurses", "gtk".
     * If it is left empty, the most suitable UI will automatically be chosen.
     **/
    YUIComponent( const string & requestedUIName = string() );

    /**
     * Destructor.
     **/
    virtual ~YUIComponent();

    /**
     * Set the requested UI name after component creation.
     * This might legitimately happen if the component creator is first called
     * with "UI" and later again with one of "qt", "ncurses", "gtk". 
     **/
    void setRequestedUIName( const string & requestedUIName );

    /**
     * Return the requested UI name.
     **/
    string requestedUIName() const { return _requestedUIName; }
    
    /**
     * Import a name space.
     **/
    virtual Y2Namespace *import( const char* name );

    /**
     * Returns the instance of the UI component 0 if none has been created yet.
     **/
    static YUIComponent * uiComponent();

    /**
     * Returns the instance of the UI or 0 if none has been created yet.
     * Note: This does _not_ create a UI on the first call; this happens in the
     * first call of a UI builtin via the YUIComponent's call handler which
     * creates a UI upon its first call and then calls the UI's call handler.
     **/
    static YUI * ui() { return _ui; }

    /**
     * Create a UI instance. The UI component normally handles that all by
     * itself when the first UI builtin is called.
     **/
    void createUI();

    /**
     * YUIComponent level call handler; this creates the actual UI instance
     * upon its first call and then hands over the function to be called to the
     * UI's call handler to make sure it is executed in the correct thread.
     **/
    YCPValue callBuiltin( void * function, int fn_argc, YCPValue fn_argv[] );
    
    /**
     * Called from generic frontend upon session close.
     * This deletes the UI.
     **/
    void result( const YCPValue & result );

    /**
     * This is called by the generic frontend after it parsed the commandline.
     * This actually creates an UI instance with createUI().
     **/
    void setServerOptions( int argc, char ** argv );

    /**
     * The name of the component - the prefix used for builtin calls like
     * UI::OpenDialog() etc.
     **/
    virtual string name() const { return string( "UI" ); }

    /**
     * The name of a macro file that might have been passed as a -macro
     * command line argument or 0 if none
     **/
    const char * macroFile() const { return _macroFile; }

    /**
     * Return 'true' if a dummy UI is used, i.e. a real UI will never be
     * instantiated. This is used in YCP testsuites to prevent UI calls
     * blocking the testsuite workflow.
     **/
    static bool useDummyUI() { return _useDummyUI; }
    
    /**
     * Set the useDummyUI flag to prevent a real UI from ever being
     * instantiated. This is used in YCP testsuites to prevent UI calls
     * blocking the testsuite workflow.
     **/
    static void setUseDummyUI( bool useDummyUI ) { _useDummyUI = useDummyUI; }
    
    /**
     * Set a callback component.
     **/
    void setCallback( Y2Component * callbackComponent )
	{ _callbackComponent = callbackComponent; }
    
    /**
     * Return the UI's callback component previously set with setCallback().
     **/
    Y2Component * getCallback() const { return _callbackComponent; }
    
    
private:

    static YUI *		_ui;
    static YUIComponent *	_uiComponent;
    static bool			_useDummyUI;

    string			_requestedUIName;
    bool			_withThreads;
    const char *		_macroFile;
    bool			_haveServerOptions;
    Y2Namespace *		_namespace;
    Y2Component *		_callbackComponent;
};

#endif // YUIComponent_h

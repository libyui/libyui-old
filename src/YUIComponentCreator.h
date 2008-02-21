/*---------------------------------------------------------------------\
|                                                                      |  
|                      __   __    ____ _____ ____                      |  
|                      \ \ / /_ _/ ___|_   _|___ \                     |  
|                       \ V / _` \___ \ | |   __) |                    |  
|                        | | (_| |___) || |  / __/                     |  
|                        |_|\__,_|____/ |_| |_____|                    |  
|                                                                      |  
|                               core system                            | 
|                                                        (C) SuSE GmbH |  
\----------------------------------------------------------------------/ 

   File:	YUIComponentCreator.h

   Authors:	Stanislav Visnovsky <visnov@suse.de>
		Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YUIComponentCreator_h
#define YUIComponentCreator_h

#include <y2/Y2ComponentCreator.h>
#include <string>

class Y2Component;
using std::string;


/**
 * Component creator that provides access to the UI
 **/
class YUIComponentCreator : public Y2ComponentCreator
{
    
public:
    /**
     * Constructor.
     **/
    YUIComponentCreator();

    virtual bool isServerCreator () const { return true; }

    /**
     * Creation function for Y2Components.
     *
     * This class can create Y2UIComponents named:
     *     - "UI"	(generic UI, for testsuites)
     *     - "ui"	(automatically choose a suitable UI)
     *     - "qt"	(Qt UI)
     *     - "ncurses"	(NCurses (text based) UI)
     *
     * In future versions also:
     *
     *     - "gtk"	(Gtk UI)
     *
     * For all other names, 0 is returned, so the Y2ComponentBroker will keep
     * on trying with other available creators. 
     **/
    virtual  Y2Component * create( const char * name ) const;

    /**
     * Creation function for non-built-in components.
     **/
    virtual Y2Component * createInLevel( const char * name, int level, int currentLevel ) const;
    
    /**
     * Name space provider function.
     * This class can provide the "UI" name space.
     *
     * For all other names, 0 is returned, so the Y2ComponentBroker will keep
     * on trying with other available creators. 
     **/
    virtual  Y2Component * provideNamespace( const char * name );

protected:

    /**
     * Internal create method.
     **/
    Y2Component * createInternal( const string & name, bool isNamespace ) const;
};


#endif // YUIComponentCreator_h

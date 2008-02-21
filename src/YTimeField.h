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

  File:		YTimeField.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YTimeField_h
#define YTimeField_h

#include "YSimpleInputField.h"

class YTimeFieldPrivate;


/**
 * Input field for entering a time in "hh:mm:ss" format.
 *
 * Derived classes are required to implement:
 *     	value()
 *	setValue()
 * See YSimpleInputField.h for details.
 **/
class YTimeField : public YSimpleInputField
{
protected:
    /**
     * Constructor.
     **/
    YTimeField( YWidget * parent, const string & label );

public:
    /**
     * Destructor.
     **/
    virtual ~YTimeField();
    
    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YTimeField"; }

private:
    
    ImplPtr<YTimeFieldPrivate> priv;
};


#endif // YTimeField_h

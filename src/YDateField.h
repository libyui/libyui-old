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

  File:		YDateField.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YDateField_h
#define YDateField_h

#include "YSimpleInputField.h"

class YDateFieldPrivate;

/**
 * Input field for entering a date.
 *
 * Derived classes are required to implement:
 *     	value()
 *	setValue()
 *
 * For both methods the date is formatted as "YYYY-MM-DD".
 * See YSimpleInputField.h for more details.
 **/
class YDateField : public YSimpleInputField
{
protected:
    /**
     * Constructor.
     **/
    YDateField( YWidget * parent, const string & label );

public:
    /**
     * Destructor.
     **/
    virtual ~YDateField();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YDateField"; }


private:

    ImplPtr<YDateFieldPrivate> priv;
};


#endif // YDateField_h

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

  File:		YCPValueWidgetID.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPValueWidgetID_h
#define YCPValueWidgetID_h

#include "YWidgetID.h"
#include <ycp/YCPValue.h>

using std::string;


/**
 * Widget ID class based on YCPValue.
 **/
class YCPValueWidgetID: public YWidgetID
{
public:

    /**
     * Constructor.
     **/
    YCPValueWidgetID( const YCPValue & value );

    /**
     * Check if this ID is equal to another.
     *
     * Reimplemented from YWidgetID.
     **/
    virtual bool isEqual( YWidgetID * otherID ) const;

    /**
     * Convert the ID value to string.
     * Used for logging and debugging.
     *
     * Reimplemented from YWidgetID.
     **/
    virtual string toString() const;

    /**
     * Return the ID value.
     **/
    YCPValue value() const;

private:
    
    YCPValue _value;
};



#endif // YCPValueWidgetID_h

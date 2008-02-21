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

  File:		YCPBuiltinCaller.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPBuiltinCaller_h
#define YCPBuiltinCaller_h

#include "YBuiltinCaller.h"
#include "ycp/YCPValue.h"

class YUIFunction;


/**
 * Helper class for transparently calling a built-in function, passing back and
 * forth YCPValues. 
 **/
class YCPBuiltinCaller: public YBuiltinCaller
{
public:
    YCPBuiltinCaller()
	: _function(0)
	{}
    
    virtual ~YCPBuiltinCaller() {}

    /**
     * Set the built-in function to be called.
     **/
    void setFunction( YUIFunction * function ) { _function = function; }
    
    /**
     * Call the built-in.
     *
     * Implemented from YBuiltinCaller.
     **/
    virtual void call();

    /**
     * Return the result of the last call.
     **/
    YCPValue result() const { return _result; }

private:

    YUIFunction *	_function;
    YCPValue		_result;
};


#endif // YCPBuiltinCaller_h

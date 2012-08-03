/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/


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

#include <yui/YBuiltinCaller.h>
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

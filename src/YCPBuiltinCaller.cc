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

  File:		YCPBuiltinCaller.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include "YCPBuiltinCaller.h"
#include "YUINamespace.h"


void
YCPBuiltinCaller::call()
{
    if ( _function )
    {
	_result   = _function->evaluateCall_int();
	_function = 0; // make sure this function is only called once
    }
}

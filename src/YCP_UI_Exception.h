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

  File:		YCP_UI_Exception.h

  Author:     	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCP_UI_Exception_h
#define YCP_UI_Exception_h


#include <ycp/YCPValue.h>
#include "YUIException.h"


/**
 * Exception class for "YCP dialog term syntax error"
 **/
class YCPDialogSyntaxErrorException: public YUIException
{
public:
    YCPDialogSyntaxErrorException( const string & msg )
	: YUIException( string( "Syntax Error:" ) + msg )
	{}

    YCPDialogSyntaxErrorException( const string   & expected,
				   const YCPValue & seen )
	: YUIException( string( "Syntax Error: " )
			+ expected
			+ string( ", seen: " ) +
			seen->toString() )
	{}

    virtual ~YCPDialogSyntaxErrorException() throw()
	{}
};


#endif // YCP_UI_Exception_h

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

  File:		YCPErrorDialog.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPErrorDialog_h
#define YCPErrorDialog_h

#include <string>
#include "YUIException.h"

using std::string;


class YCPErrorDialog
{
public:
    /**
     * Post an error dialog with the specified heading text and the specified
     * UI exception and wait for the user to confirm it.
     **/
    static void exceptionDialog( const string & headingText,
				 const YUIException & exception ) throw();

private:
    YCPErrorDialog() {}
};


#endif // YCPErrorDialog_h


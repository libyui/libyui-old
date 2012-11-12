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


  File:		YCPWizardCommandParser.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPWizardCommandParser_h
#define YCPWizardCommandParser_h

#include <ycp/YCPTerm.h>
#include <ycp/YCPString.h>
#include <ycp/YCPInteger.h>
#include <ycp/YCPBoolean.h>
#include <ycp/YCPVoid.h>

class YWizard;
class YPushButton;
class YWidget;


/**
 * Parser for UI::WizardCommand()
 **/
class YCPWizardCommandParser
{
public:
    /**
     * Parse and execute one wizard command.
     *
     * Returns 'true' on success, 'false' on failure.
     **/
    static bool parseAndExecute( YWizard *	 wizard,
				 const YCPTerm & command );

private:

    /**
     * Check if 'term' matches wizard command 'declaration'.
     * 'declaration' is a function prototype like this:
     *
     *		myFunction ( string, boolean, string )
     *
     * Void functions are declared without any parameters:
     *
     *		myFunction ()
     *
     * Function names must be unique. They cannot be overloaded.
     **/
    static bool isCommand( string declaration, const YCPTerm & term );

    /**
     * Remove all whitespace (leading, trailing, and internal) from a string.
     **/
    static string stripWhitespace( const string & orig );
    
    /**
     * Return argument number 'argNo' from 'term' as string.
     **/
    static string stringArg( const YCPTerm & term, int argNo );

    /**
     * Return argument number 'argNo' from 'term' as bool.
     **/
    static bool boolArg( const YCPTerm & term, int argNo );

    /**
     * Return argument as type 'any' (plain YCPValue)
     **/
    static YCPValue anyArg( const YCPTerm & term, int argNo );

    /**
     * Set a widget's ID if 'widget' is non-null.
     **/
    static void setId( YWidget * widget, const YCPValue & id );

    /**
     * Enable or disable a widget if it is non-null.
     * Disabling the wizard's "Next" button is ignored if it is currently
     * proteced with YWizard::ProtectNextButton().  
     **/
    static void enable( YWizard * wizard, YWidget * widget, bool enabled );

    /**
     * Set the keyboard focus to a widget if it is non-null.
     **/
    static void setFocus( YWidget * widget );

    /**
     * Constructor (disabled).
     **/
    YCPWizardCommandParser() {}
};

#endif // YCPWizardCommandParser_h

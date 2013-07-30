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

  File:		YCPWizardCommandParser.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#define y2log_component "ui-wizard"
#include <ycp/y2log.h>

#include "YCPWizardCommandParser.h"
#include "YCPValueWidgetID.h"
#include <yui/YWizard.h>
#include <yui/YPushButton.h>


bool
YCPWizardCommandParser::parseAndExecute( YWizard * wizard, const YCPTerm & cmd )
{
    if ( isCommand( "SetHelpText	  ( string )", cmd ) )	{ wizard->setHelpText	( stringArg( cmd, 0 ) );		return true; }
    if ( isCommand( "SetDialogIcon	  ( string )", cmd ) )	{ wizard->setDialogIcon ( stringArg( cmd, 0 ) );		return true; }
    if ( isCommand( "SetDialogTitle	  ( string )", cmd ) )	{ wizard->setDialogTitle( stringArg( cmd, 0 ) );		return true; }
    if ( isCommand( "SetDialogHeading	  ( string )", cmd ) )	{ wizard->setDialogHeading( stringArg( cmd, 0 ) );		return true; }

    if ( isCommand( "SetCurrentStep	  ( string )", cmd ) )	{ wizard->setCurrentStep( stringArg( cmd, 0 ) );		return true; }
    if ( isCommand( "AddStep ( string, string )"     , cmd ) )	{ wizard->addStep( stringArg( cmd, 0 ), stringArg( cmd, 1 )); 	return true; }
    if ( isCommand( "AddStepHeading	  ( string )", cmd ) )	{ wizard->addStepHeading( stringArg( cmd, 0 ) );		return true; }
    if ( isCommand( "DeleteSteps()"		     , cmd ) )	{ wizard->deleteSteps();					return true; }
    if ( isCommand( "UpdateSteps()"		     , cmd ) )	{ wizard->updateSteps();					return true; }

    if ( isCommand( "SetAbortButtonLabel  ( string )", cmd ) )	{ wizard->setButtonLabel( wizard->abortButton(),
											  stringArg( cmd, 0 ) );		return true; }
    
    if ( isCommand( "SetBackButtonLabel	  ( string )", cmd ) )	{ wizard->setButtonLabel( wizard->backButton(),
											  stringArg( cmd, 0 ) );		return true; }
    
    if ( isCommand( "SetNextButtonLabel	  ( string )", cmd ) )	{ wizard->setButtonLabel( wizard->nextButton(),
											  stringArg( cmd, 0 ) );		return true; }
    
    if ( isCommand( "SetCancelButtonLabel ( string )", cmd ) )	{ wizard->setButtonLabel( wizard->abortButton(),
											  stringArg( cmd, 0 ) );		return true; }
    
    if ( isCommand( "SetAcceptButtonLabel ( string )", cmd ) )	{ wizard->setButtonLabel( wizard->nextButton(),
											  stringArg( cmd, 0 ) );		return true; }

    if ( isCommand( "SetAbortButtonID	  ( any )"   , cmd ) )	{ setId( wizard->abortButton(),	anyArg( cmd, 0 ) );		return true; }
    if ( isCommand( "SetBackButtonID	  ( any )"   , cmd ) )	{ setId( wizard->backButton(),	anyArg( cmd, 0 ) );		return true; }
    if ( isCommand( "SetNextButtonID	  ( any )"   , cmd ) )	{ setId( wizard->nextButton(),	anyArg( cmd, 0 ) );		return true; }

    if ( isCommand( "EnableBackButton	  ( bool )"  , cmd ) )	{ enable( wizard, wizard->backButton(),	boolArg( cmd, 0 ) );	return true; }
    if ( isCommand( "EnableNextButton	  ( bool )"  , cmd ) )	{ enable( wizard, wizard->nextButton(),	boolArg( cmd, 0 ) );	return true; }
    if ( isCommand( "EnableAbortButton	  ( bool )"  , cmd ) )	{ enable( wizard, wizard->abortButton(),boolArg( cmd, 0 ) );	return true; }
    if ( isCommand( "ProtectNextButton	  ( bool )"  , cmd ) )	{ wizard->protectNextButton( boolArg( cmd, 0 ) );		return true; }

    if ( isCommand( "SetFocusToNextButton ()"	     , cmd ) )	{ setFocus( wizard->nextButton() );				return true; }
    if ( isCommand( "SetFocusToBackButton ()"	     , cmd ) )	{ setFocus( wizard->backButton() );				return true; }

    if ( isCommand( "DeleteTreeItems()"		     , cmd ) )	{ wizard->deleteTreeItems();					return true; }
    if ( isCommand( "SelectTreeItem( string )"	     , cmd ) )	{ wizard->selectTreeItem( stringArg( cmd, 0 ) );		return true; }
    if ( isCommand( "AddTreeItem( string, string, string )", cmd ) )	{ wizard->addTreeItem	( stringArg( cmd, 0 ),
												  stringArg( cmd, 1 ),
												  stringArg( cmd, 2 )	);	return true; }

    if ( isCommand( "AddMenu	  ( string, string )"	      , cmd ) ) { wizard->addMenu	( stringArg( cmd, 0 ),
												  stringArg( cmd, 1 ) );	return true; }

    if ( isCommand( "AddSubMenu	  ( string, string, string )" , cmd ) ) { wizard->addSubMenu	( stringArg( cmd, 0 ),
												  stringArg( cmd, 1 ),
												  stringArg( cmd, 2 ) );	return true; }

    if ( isCommand( "AddMenuEntry ( string, string, string )" , cmd ) ) { wizard->addMenuEntry	( stringArg( cmd, 0 ),
												  stringArg( cmd, 1 ),
												  stringArg( cmd, 2 ) );	return true; }

    if ( isCommand( "AddMenuSeparator ( string )"	     , cmd ) )	{ wizard->addMenuSeparator( stringArg( cmd, 0 ) );	return true; }
    if ( isCommand( "DeleteMenus ()"			     , cmd ) )	{ wizard->deleteMenus();				return true; }
    if ( isCommand( "ShowReleaseNotesButton( string, string )", cmd ) )	{ wizard->showReleaseNotesButton( stringArg( cmd, 0 ),
													  stringArg( cmd, 1 )); return true; }
    if ( isCommand( "HideReleaseNotesButton()"		     , cmd ) )	{ wizard->hideReleaseNotesButton();			return true; }
    if ( isCommand( "RetranslateInternalButtons()"	     , cmd ) )	{ wizard->retranslateInternalButtons() ;		return true; }
    if ( isCommand( "Ping()"				     , cmd ) )	{ wizard->ping() ;					return true; }
    y2error( "Undefined wizard command: %s", cmd->toString().c_str() );
    return false;
}


bool
YCPWizardCommandParser::isCommand( string declaration, const YCPTerm & term )
{
    // myfunc   ( arg1, arg2, arg3 ) -> myfunc(arg1,arg2,arg3)
    declaration = stripWhitespace( declaration );
    // y2debug( "stripped declaration: \"%s\"", declaration.c_str() );


    // Extract function name
    string::size_type parenPos = declaration.find( '(' );

    if ( parenPos == string::npos ||		// No ( at all in declaration
	 parenPos < 1 )				// or nothing before (
	return false;

    string functionName = declaration.substr( 0, parenPos );
    // y2debug( "function name: \"%s\"", functionName.c_str() );
    
    // Check function name
    if ( term->name() != functionName )
	return false;

    //
    // Check arguments
    //

    string::size_type endParenPos = declaration.rfind( ')' );

    if ( endParenPos == string::npos ||
	 parenPos >= endParenPos )
    {
	y2error( "Bad declaration: %s", declaration.c_str() );
	return false;
    }

    // Extract argument declarations:  myfunc(arg1,arg2,arg3) -> arg1,arg2,arg3
    string argDecl = declaration.substr( parenPos+1, endParenPos - parenPos - 1 );
    // y2debug( "Full arg decl: \"%s\"", argDecl.c_str() );

    //
    // Split argument declarations into vector<string>
    //

    vector<string> argDeclList;

    if ( argDecl.size() > 0 )		// handle empty argDecl: myfunc()
    {
	string::size_type argBegin = 0;
	string::size_type argEnd   = 0;

	while ( argEnd != string::npos )
	{
	    argEnd = argDecl.find( ',', argBegin );
	    string::size_type len = ( argEnd == string::npos ? argDecl.size() : argEnd );
	    len -= argBegin;
		
	    string arg = argDecl.substr( argBegin, len );
	    //y2debug( "\tFound arg decl: \"%s\"", arg.c_str() );
	    argDeclList.push_back( arg );
	    argBegin = argEnd+1;
	} 
    }


    //
    // Check number of arguments
    //

    if ( argDeclList.size() != (unsigned) term->size() )
    {
	y2error( "Bad arguments for wizard command %s : %s",
		 declaration.c_str(), term->toString().c_str() );
	return false;
    }


    //
    // Check each individual argument
    //

    bool ok = true;
    int  i  = 0;

    for ( vector<string>::const_iterator it = argDeclList.begin();
	  it != argDeclList.end() && ok;
	  ++it, ++i )
    {
	string wanted = *it;
	YCPValue seen  = term->value( i );

	if	( wanted == "string"	)	ok = seen->isString();
	else if ( wanted == "boolean"	)	ok = seen->isBoolean();
	else if ( wanted == "bool"	)	ok = seen->isBoolean();
	else if ( wanted == "list"	)	ok = seen->isList();
	else if ( wanted == "map"	)	ok = seen->isMap();
	else if ( wanted == "integer"	)	ok = seen->isInteger();
	else if ( wanted == "int"	)	ok = seen->isInteger();
	else if ( wanted == "any"	)	ok = true;
	else
	{
	    y2error( "Bad declaration for wizard command %s : Unknown type \"%s\"",
		     declaration.c_str(), wanted.c_str() );
	}
    }

    if ( ! ok )
    {
	y2error( "Bad arguments for wizard command %s : %s",
		 declaration.c_str(), term->toString().c_str() );
    }

    if ( ok )
    {
	y2debug( "Recognized wizard command %s : %s",
		 declaration.c_str(), term->toString().c_str() );
    }

    return ok;
}


string
YCPWizardCommandParser::stripWhitespace( const string & orig )
{
    // This could be done with std::string operations, but pure C is a lot more
    // efficient with this kind of thing, even with the added overhead of
    // allocating (and later freeing) heap memory.

    const char * src	= orig.c_str();
    char *	 result = strdup( src );
    char * 	 dest  	= result;

    if ( ! result )
    {
	y2error( "strdup() failed" );
	return orig;
    }

    while ( *src != '\0' )
    {
	if ( *src != ' '  &&
	     *src != '\t' &&
	     *src != '\n'   )
	    *dest++ = *src;	// Copy all non-whitespace characters

	++src;
    }

    *dest = '\0';

    string resultStr( result );
    free( result );

    return resultStr;
}


string
YCPWizardCommandParser::stringArg( const YCPTerm & term, int argNo )
{
    if ( term->size() > argNo )
    {
	YCPValue arg( term->value( argNo ) );

	if ( arg->isString() )
	    return arg->asString()->value();
    }

    y2error( "Couldn't convert arg #%d of '%s' to string", argNo, term->toString().c_str() );
    return "";
}


bool
YCPWizardCommandParser::boolArg( const YCPTerm & term, int argNo )
{
    if ( term->size() > argNo )
    {
	YCPValue arg( term->value( argNo ) );

	if ( arg->isBoolean() )
	    return arg->asBoolean()->value();
    }

    y2error( "Couldn't convert arg #%d of '%s' to bool", argNo, term->toString().c_str() );
    return false;
}


YCPValue
YCPWizardCommandParser::anyArg( const YCPTerm & term, int argNo )
{
    if ( term->size() > argNo )
    {
	return term->value( argNo );
    }

    return YCPVoid();
}


void
YCPWizardCommandParser::setId( YWidget * widget, const YCPValue & id )
{
    if ( widget )
	widget->setId( new YCPValueWidgetID( id ) );
    else
	y2error( "NULL widget" );
}


void
YCPWizardCommandParser::enable( YWizard * wizard, YWidget * widget, bool enabled )
{
    if ( widget )
    {
	if ( widget == wizard->nextButton()  &&	// Ignore any attempt
	     wizard->nextButtonIsProtected() &&	// to disable a protected "Next" button
	     ! enabled )
	    return;

	widget->setEnabled( enabled );
    }
    else
	y2error( "NULL widget" );
}


void
YCPWizardCommandParser::setFocus( YWidget * widget )
{
    if ( widget )
	widget->setKeyboardFocus();
    else
	y2error( "NULL widget" );
}


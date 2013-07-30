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

   File:	YUINamespace.h

   Authors:	Stanislav Visnovsky <visnov@suse.cz>
		Martin Vidner <mvidner@susez.cz>

   Maintainer:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <y2/Y2Namespace.h>
#include <y2/Y2Function.h>
#include <ycp/YCPBoolean.h>
#include <ycp/YCPInteger.h>
#include <ycp/YCPVoid.h>
#include <ycp/YCPString.h>
#include <ycp/YCPCode.h>
#include <ycp/StaticDeclaration.h>

#include <yui/YUI.h>
#include "YUIComponent.h"

class YUINamespace;

/**
 * One function call of the UI:: namespace
 **/
class YUIFunction: public Y2Function
{
protected:

    unsigned int m_position;
    YUINamespace* m_instance;
    YUIComponent* m_comp;
    bool m_play_macro_blocks;
    YCPValue m_param1;
    YCPValue m_param2;
    YCPValue m_param3;
    YCPValue m_param4;
    YCPValue m_param5;
public:

    YUIFunction (YUINamespace* instance, YUIComponent* comp, unsigned int pos, bool play_macro_blocks);

    bool attachParameter (const YCPValue& arg, const int position);
    constTypePtr wantedParameterType () const;
    bool appendParameter (const YCPValue& arg);
    bool finishParameters();
    YCPValue evaluateCall();
    YCPValue evaluateCall_int();
    bool reset();
    string name () const;
};


/**
 * One function call of the UI:: namespace:
 * A function where the signature is not known beforehand
 **/
class YUIOverloadedFunction: public YUIFunction
{
    /**
     * A range of SymbolEntries with a matching name.
     **/
    vector<SymbolEntryPtr>::iterator m_candidates_b;
    vector<SymbolEntryPtr>::iterator m_candidates_e;

public:
    YUIOverloadedFunction( YUINamespace *			instance,
			    YUIComponent  * 			comp,
			    unsigned 				pos_offset,
			    vector<SymbolEntryPtr>::iterator 	candidates_b,
			    vector<SymbolEntryPtr>::iterator 	candidates_e,
			    bool 				play_macro_blocks );

    // attachParameter, appendParameter, reset from parent: fine
    // wantedParameterType from parent: kind of fine

    bool finishParameters();
};


class YUINamespace: public Y2Namespace
{
private:
    void registerFunctions();

    vector<string> _registered_functions;

    YUIComponent* m_comp;

public:
    /**
     * Constructor.
     */
    YUINamespace (YUIComponent* comp);

    /**
     * Destructor.
     */
    virtual ~YUINamespace();

    virtual const string name() const
    { return "UI"; }

    virtual const string filename() const
    { return "UI"; }

    virtual string toString() const
    { return "// can't convert to string with toString"; }

    virtual YCPValue evaluate( bool cse = false )
    {
	if ( cse )
	    return YCPNull();
	else
	    return YCPVoid();
    }

    virtual Y2Function* createFunctionCall (const string name, constFunctionTypePtr type);




    /* TYPEINFO: void (string) */
    YCPValue SetLanguage( const YCPString & language );

    /* TYPEINFO: void (string, string) */
    YCPValue SetLanguage( const YCPString & language, const YCPString & encoding );

    /* TYPEINFO: string () */
    YCPValue GetProductName();

    /* TYPEINFO: void (string) */
    YCPValue SetProductName( const YCPString & name );

    /* TYPEINFO: void (string, string, string, string, string) */
    YCPValue SetConsoleFont ( const YCPString & console_magic, const YCPString & font, const YCPString & screen_map, const YCPString & unicode_map, const YCPString & encoding );

    /* TYPEINFO: void () */
    YCPValue SetKeyboard();

    /* TYPEINFO: string (boolean) */
    YCPValue GetLanguage( const YCPBoolean & strip );

    /* TYPEINFO: any() */
    YCPValue UserInput();

    /* TYPEINFO: any () */
    YCPValue PollInput();

    /* TYPEINFO: any (integer) */
    YCPValue TimeoutUserInput( const YCPInteger& timeout );

    /* TYPEINFO: map<string,any> () */
    YCPValue WaitForEvent();

    /* TYPEINFO: map<string,any> (integer) */
    YCPValue WaitForEvent( const YCPInteger & timeout );

    /* TYPEINFO: void(term) */
    YCPValue OpenDialog( const YCPTerm & dialog_term );

    /* TYPEINFO: void(term,term) */
    YCPValue OpenDialog( const YCPTerm & opts, const YCPTerm & dialog_term );

    /* TYPEINFO: boolean() */
    YCPValue CloseDialog();

    /* TYPEINFO: boolean (symbol, symbol, any) */
    YCPValue ChangeWidget( const YCPSymbol & widget_id, const YCPSymbol & property, const YCPValue & /*nil*/ new_value );

    /* TYPEINFO: boolean (term, symbol, any) */
    YCPValue ChangeWidget( const YCPTerm & widget_id, const YCPSymbol & property, const YCPValue & /*nil*/ new_value );

    // No 'nil' permitted for new_value here
    /* TYPEINFO: boolean (term, term, any) */
    YCPValue ChangeWidget( const YCPTerm & widget_id, const YCPTerm & property, const YCPValue & new_value );

    /* TYPEINFO: any (symbol, symbol) */
    YCPValue QueryWidget( const YCPSymbol & widget_id, const YCPSymbol & property );

    /* TYPEINFO: any (symbol, term) */
    YCPValue QueryWidget( const YCPSymbol & widget_id, const YCPTerm & property );

    /* TYPEINFO: any (term, symbol) */
    YCPValue QueryWidget( const YCPTerm & widget_id, const YCPSymbol & property );

    /* TYPEINFO: any (term, term) */
    YCPValue QueryWidget( const YCPTerm & widget_id, const YCPTerm & property );

    /* TYPEINFO: boolean (symbol, term) */
    YCPValue ReplaceWidget( const YCPSymbol & widget_id, const YCPTerm & new_widget );

    /* TYPEINFO: boolean (term, term) */
    YCPValue ReplaceWidget( const YCPTerm & widget_id, const YCPTerm & new_widget );

    /* TYPEINFO: boolean (symbol) */
    YCPValue SetFocus( const YCPSymbol & widget_id );

    /* TYPEINFO: boolean (term) */
    YCPValue SetFocus( const YCPTerm & widget_id );

    /* TYPEINFO: integer (string) */
    YCPValue RunInTerminal( const YCPString & command );

    /* TYPEINFO: void () */
    YCPValue BusyCursor();

    /* TYPEINFO: void () */
    YCPValue RedrawScreen();

    /* TYPEINFO: void () */
    YCPValue NormalCursor();

    /* TYPEINFO: void (string) */
    YCPValue MakeScreenShot( const YCPString & filename );

    /* TYPEINFO: void () */
    YCPValue MakeScreenShot();

    /* TYPEINFO: void () */
    YCPValue DumpWidgetTree();

    /* TYPEINFO: void () */
    YCPValue Beep();

    /* TYPEINFO: void (string) */
    YCPValue RecordMacro( const YCPString & filename );

    /* TYPEINFO: void () */
    YCPValue StopRecordMacro();

    /* TYPEINFO: void (string) */
    YCPValue PlayMacro( const YCPString & filename );

    /* TYPEINFO: void () */
    YCPValue FakeUserInput();

    /* TYPEINFO: void (any) */
    YCPValue FakeUserInput( const YCPValue & next_input );

    /* TYPEINFO: string (symbol) */
    YCPValue Glyph( const YCPSymbol & glyphSym  );

    /* TYPEINFO: map<string,any> () */
    YCPValue GetDisplayInfo();

    /* TYPEINFO: void () */
    YCPValue RecalcLayout();

    /* TYPEINFO: void () */
    YCPValue PostponeShortcutCheck();

    /* TYPEINFO: void () */
    YCPValue CheckShortcuts();

    /* TYPEINFO: boolean (symbol) */
    YCPValue WidgetExists( const YCPSymbol & widget_id );

    /* TYPEINFO: boolean (term) */
    YCPValue WidgetExists( const YCPTerm & widget_id );

    /* TYPEINFO: any (term) */
    YCPValue RunPkgSelection( const YCPValue & widget_id );

    /* TYPEINFO: string (string, string) */
    YCPValue AskForExistingDirectory( const YCPString & startDir, const YCPString & headline );

    /* TYPEINFO: string (string, string, string) */
    YCPValue AskForExistingFile( const YCPString & startWith, const YCPString & filter, const YCPString & headline );

    /* TYPEINFO: string (string, string, string) */
    YCPValue AskForSaveFileName( const YCPString & startWith, const YCPString & filter, const YCPString & headline );

    /* TYPEINFO: void (map<any,any>) */
    YCPValue SetFunctionKeys( const YCPMap & new_fkeys );

    /* TYPEINFO: any (string, string, string) */
    YCPValue Recode( const YCPString & from, const YCPString & to, const YCPString & text );

    /* TYPEINFO: boolean (symbol) */
    YCPValue HasSpecialWidget( const YCPSymbol & widget );

    /* TYPEINFO: boolean (term) */
    YCPValue WizardCommand( const YCPTerm & command );

    /* TYPEINFO: boolean () */
    YCPBoolean TextMode();

    /* TYPEINFO: boolean (term) */	
    YCPBoolean OpenContextMenu( const YCPTerm & term );


    
    YCPValue
    CallHandler( void * ptr, int argc, YCPValue argv[] );
};

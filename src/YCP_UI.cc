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

  File:		YCP_UI.cc

  Authors:	Stefan Hundhammer <sh@suse.de>

/-*/


#define VERBOSE_REPLACE_WIDGET 		0
#define VERBOSE_EVENTS			0

#include <stdio.h>
#include <string.h>

#define y2log_component "ui"
#include <ycp/y2log.h>	// ycperror()

#define YUILogComponent "ui"
#include "YUILog.h"

#include <Y2.h>

#include "YCP_UI.h"
#include "YUI.h"
#include "YUIComponent.h"
#include "YUIException.h"
#include "YUISymbols.h"
#include "YApplication.h"
#include "YCPDialogParser.h"
#include "YCPErrorDialog.h"
#include "YCPEvent.h"
#include "YCPItemParser.h"
#include "YCPMacroPlayer.h"
#include "YCPMacroRecorder.h"
#include "YCPMenuItemParser.h"
#include "YCPPropertyHandler.h"
#include "YCPValueWidgetID.h"
#include "YCPWizardCommandParser.h"
#include "YCP_util.h"
#include "YCheckBox.h"
#include "YDialog.h"
#include "YEvent.h"
#include "YMacro.h"
#include "YOptionalWidgetFactory.h"
#include "YReplacePoint.h"
#include "YShortcut.h"
#include "YWidget.h"
#include "YWidgetFactory.h"
#include "YWizard.h"

using std::string;

std::deque<YCPValue> YCP_UI::_fakeUserInputQueue;


/**
 * @builtin HasSpecialWidget
 * @short Checks for support of a special widget type.
 * @description
 * Checks for support of a special widget type. Use this prior to creating a
 * widget of this kind. Do not use this to check for ordinary widgets like
 * PushButton etc. - just the widgets where the widget documentation explicitly
 * states it is an optional widget not supported by all UIs.
 *
 * Returns true if the UI supports the special widget and false if not.
 */

YCPValue YCP_UI::HasSpecialWidget( const YCPSymbol & widget )
{
    YOptionalWidgetFactory * fact = YUI::optionalWidgetFactory();

    if ( ! fact )
	return YCPBoolean( false );

    bool   hasWidget = false;
    string symbol    = widget->symbol();

    if	    ( symbol == YUISpecialWidget_DummySpecialWidget	)	hasWidget = fact->hasDummySpecialWidget();
    else if ( symbol == YUISpecialWidget_BarGraph		)	hasWidget = fact->hasBarGraph();
    else if ( symbol == YUISpecialWidget_DumbTab		)	hasWidget = fact->hasDumbTab();
    else if ( symbol == YUISpecialWidget_DownloadProgress	)	hasWidget = fact->hasDownloadProgress();
    else if ( symbol == YUISpecialWidget_HMultiProgressMeter	)	hasWidget = fact->hasMultiProgressMeter();
    else if ( symbol == YUISpecialWidget_VMultiProgressMeter	)	hasWidget = fact->hasMultiProgressMeter();
    else if ( symbol == YUISpecialWidget_Slider			)	hasWidget = fact->hasSlider();
    else if ( symbol == YUISpecialWidget_PatternSelector	)	hasWidget = fact->hasPatternSelector();
    else if ( symbol == YUISpecialWidget_PartitionSplitter	)	hasWidget = fact->hasPartitionSplitter();
    else if ( symbol == YUISpecialWidget_SimplePatchSelector	)	hasWidget = fact->hasSimplePatchSelector();
    else if ( symbol == YUISpecialWidget_Wizard			)	hasWidget = fact->hasWizard();
    else if ( symbol == YUISpecialWidget_Date			)	hasWidget = fact->hasDateField();
    else if ( symbol == YUISpecialWidget_Time			)	hasWidget = fact->hasTimeField();
    else if ( symbol == YUISpecialWidget_TimezoneSelector	)	hasWidget = fact->hasTimezoneSelector();
    else if ( symbol == YUISpecialWidget_Graph			)	hasWidget = fact->hasGraph();
    else if ( symbol == YUISpecialWidget_ContextMenu		)	hasWidget = fact->hasContextMenu();
    else
    {
	yuiError() << "HasSpecialWidget(): Unknown special widget: " << symbol << endl;
	return YCPNull();
    }

    return YCPBoolean( hasWidget );
}


/**
 * @builtin SetLanguage
 * @short Sets the language of the UI
 * @description
 * Tells the UI that the user has selected another language.
 * If the UI has any language dependend output that language
 * setting is honored. <tt>lang</tt> is an ISO language string,
 * such as <tt>de</tt> or <tt>de_DE</tt>. It is required
 * to specify an encoding scheme, since not all user interfaces
 * are capable of UTF-8.
 * @param string lang Language selected by user
 * @optarg string encoding
 * @return void
 *
 * @usage SetLanguage( "de_DE@euro" )
 * @usage SetLanguage( "en_GB" )
 */

void YCP_UI::SetLanguage( const YCPString & language, const YCPString & enc )
{
    string encoding = enc.isNull() ? "" : enc->value();
    YUI::app()->setLanguage( language->value(), encoding);
}


/**
 * @builtin GetProductName
 * @short Gets Product Name
 * @description
 * Returns the current product name ("SuSE Linux", "United Linux", etc.) for
 * display in dialogs. This can be set with SetProductName().
 *
 * Note: In help texts in RichText widgets, a predefined macro &amp;product;
 * can be used for the same purpose.
 *
 * @return string Product Name
 * @usage sformat( "Welcome to %1", GetProductName() );
 **/
YCPString YCP_UI::GetProductName()
{
    return YCPString( YUI::app()->productName() );
}


/**
 * @builtin SetProductName
 * @short Sets Product Name
 * @description
 * Sets the current product name ("SuSE Linux", "United Linux", etc.) for
 * displaying in dialogs and in RichText widgets (for help text) with the RichText
 * &amp;product; macro.
 *
 * This product name should be concise and meaningful to the user and not
 * cluttered with detailed version information. Don't use something like
 * "SuSE Linux 12.3-i786 Professional". Use something like "SuSE Linux"
 * instead.
 *
 * This information can be retrieved with the GetProductName() builtin.
 * @param string prod
 * @return void
 *
 * @usage SetProductName( "SuSE HyperWall" );
 **/
void YCP_UI::SetProductName( const YCPString & name )
{
    YUI::app()->setProductName( name->value() );
}


/**
 * @builtin SetConsoleFont
 * @short Sets Console Font
 * @description
 * Switches the text console to the specified font.
 * See the setfont(8) command and the console HowTo for details.
 * @see setfont(8)
 * @param string console_magic
 * @param string font
 * @param string screen_map
 * @param string unicode_map
 * @param string language
 * @return void
 *
 * @usage SetConsoleFont( "( K", "lat2u-16.psf", "latin2u.scrnmap", "lat2u.uni", "latin1" )
 */

void YCP_UI::SetConsoleFont( const YCPString & console_magic,
				  const YCPString & font,
				  const YCPString & screen_map,
				  const YCPString & unicode_map,
				  const YCPString & language )
{
    YUI::app()->setConsoleFont( console_magic->value(),
				font->value(),
				screen_map->value(),
				unicode_map->value(),
				language->value() );
}


/**
 * @builtin RunInTerminal
 * @short runs external program in the same terminal
 * @description
 * Use this builtin if you want to run an external program from the NCcurses UI
 * as a separate process. It saves the current window layout and runs the
 * external program in the same terminal. When done, it restores the original
 * window layout and returns the exit code of the external program.
 * When called from the Qt UI, an error message is printed to the log.
 * @param string external_program
 * return integer
 *
 * @usage RunInTerminal("/bin/bash")
 */

YCPInteger YCP_UI::RunInTerminal(const YCPString & command )
{
    return YCPInteger( YUI::app()->runInTerminal( command->value() ) );
}


/**
 * @builtin SetKeyboard
 * @short Sets Keyboard
 *
 * @return void
 * @usage SetKeyboard()
 */
void YCP_UI::SetKeyboard( )
{
    YUI::app()->initConsoleKeyboard();
}


/**
 * @builtin GetLanguage
 * @short Gets Language
 * @description
 * Retrieves the current language setting from of the user interface.  Since
 * YaST2 is a client server architecture, we distinguish between the language
 * setting of the user interface and that of the configuration modules. If the
 * module or the translator wants to know which language the user currently
 * uses, it can call <tt>GetLanguage</tt>. The return value is an ISO language
 * code, such as "de" or "de_DE".
 *
 * If "strip_encoding" is set to "true", all encoding or similar information is
 * cut off, i.e. everything from the first "." or "@" on. Otherwise the current
 * contents of the "LANG" environment variable is returned (which very likely
 * ends with ".UTF-8" since this is the encoding YaST2 uses internally).
 *
 * @param boolean strip_encoding
 * @return string
 *
 */

YCPString YCP_UI::GetLanguage( const YCPBoolean & strip )
{
    return YCPString( YUI::app()->language( strip->value() ) );
}



/**
 * @builtin UserInput
 * @short User Input
 * @description
 * Waits for the user to click some button, close the window or
 * activate some widget that has the <tt>`notify</tt> option set.
 * The return value is the id of the widget that has been selected
 * or <tt>`cancel</tt> if the user selected the implicit cancel
 * button (for example he closes the window).
 *
 * @return any
 */
YCPValue YCP_UI::UserInput()
{
#if VERBOSE_EVENTS
    yuiDebug() << "UI::UserInput()" << endl;
#endif

    return doUserInput( YUIBuiltin_UserInput,
			0,		// timeout_millisec
			true,		// wait
			false );	// detailed
}


/**
 * @builtin PollInput
 * @short Poll Input
 * @description
 * Doesn't wait but just looks if the user has clicked some
 * button, has closed the window or has activated
 * some widget that has the <tt>`notify</tt> option set. Returns
 * the id of the widget that has been selected
 * or <tt>`cancel</tt> if the user selected the implicit cancel
 * button (for example he closes the window). Returns nil if no
 * user input has occured.
 *
 * @return any
 *
 */
YCPValue YCP_UI::PollInput()
{
#if VERBOSE_EVENTS
    yuiDebug() << "UI::PollInput()" << endl;
#endif

    return doUserInput( YUIBuiltin_PollInput,
			0,		// timeout_millisec
			false,		// wait
			false );	// detailed
}


/**
 * @builtin TimeoutUserInput
 * @short User Input with Timeout
 * @description
 * Waits for the user to click some button, close the window or
 * activate some widget that has the <tt>`notify</tt> option set
 * or until the specified timeout is expired.
 * The return value is the id of the widget that has been selected
 * or <tt>`cancel</tt> if the user selected the implicit cancel
 * button (for example he closes the window).
 * Upon timeout, <tt>`timeout</tt> is returned.
 *
 * @param integer timeout_millisec
 * @return any
 */
YCPValue YCP_UI::TimeoutUserInput( const YCPInteger & timeout )
{
    long timeout_millisec = timeout->value();

#if VERBOSE_EVENTS
    yuiDebug() << "UI::TimeoutUserInput( " << timeout_millisec << " )" << endl;
#endif

    return doUserInput( YUIBuiltin_TimeoutUserInput,
			timeout_millisec,
			true,			// wait
			false );		// detailed
}


/**
 * @builtin WaitForEvent
 * @short Waits for Event
 * @description
 * Extended event handling - very much like UserInput(), but returns much more
 * detailed information about the event that occured in a map.
 *
 * @optarg timeout_millisec
 * @return map
 */
YCPValue YCP_UI::WaitForEvent( const YCPInteger & timeout )
{
    long timeout_millisec = 0;

    if ( ! timeout.isNull() )
    {
	timeout_millisec = timeout->value();
    }

#if VERBOSE_EVENTS
    yuiDebug() << "UI::WaitForEvent( " << timeout_millisec << " )" << endl;
#endif

    return doUserInput( YUIBuiltin_WaitForEvent,
			timeout_millisec,
			true,			// wait
			true );			// detailed
}




YCPValue YCP_UI::doUserInput( const char * 	builtin_name,
			      long 		timeout_millisec,
			      bool 		wait,
			      bool 		detailed )
{
    // Plausibility check for timeout

    if ( timeout_millisec < 0 )
    {
	yuiError() << builtin_name << "(): Invalid value " << timeout_millisec
		   << " for timeout - assuming 0"
		   << endl;

	timeout_millisec = 0;
    }

    YEvent *	event = 0;
    YCPValue 	input = YCPVoid();

    try
    {
	YDialog * dialog = YDialog::currentDialog();

	// Check for leftover postponed shortcut check

	if ( dialog->shortcutCheckPostponed() )
	{
	    yuiError() << "Missing UI::CheckShortcuts() before UI::" << builtin_name
		       << "() after UI::PostponeShortcutCheck()!"
		       << endl;

	    dialog->checkShortcuts( true );
	}


	// Handle events

	if ( _fakeUserInputQueue.empty() )
	{
	    if ( wait )
		event = dialog->waitForEvent( timeout_millisec );
	    else
		event = dialog->pollEvent();

	    if ( event )
	    {
		YCPEvent ycpEvent( event );

		if ( detailed )
		    input = ycpEvent.eventMap();
		else
		    input = ycpEvent.eventId();

#if VERBOSE_EVENTS
		yuiDebug() << "Got regular event from keyboard / mouse: " << input << endl;
#endif
	    }
	}
	else // _fakeUserInputQueue contains elements -> use the first one
	{
	    // Handle macro playing

	    input = _fakeUserInputQueue.front();
	    yuiDebug() << "Using event from fakeUserInputQueue: "<< input << endl;
	    _fakeUserInputQueue.pop_front();
	}

	// Handle macro recording

	if ( YMacro::recording() )
	{
	    YCPMacroRecorder * macroRecorder = dynamic_cast<YCPMacroRecorder *> ( YMacro::recorder() );

	    if ( macroRecorder )
	    {
		if ( ! input->isVoid() || wait )	// Don't record empty PollInput() calls
		{
		    macroRecorder->beginBlock();
		    dialog->saveUserInput( macroRecorder );
		    macroRecorder->recordUserInput( input );
		    macroRecorder->endBlock();
		}
	    }
	}
    }
    catch ( YUIException & exception )
    {
	YUI_CAUGHT( exception );
	YCPErrorDialog::exceptionDialog( "Internal Error", exception );
	YUI_RETHROW( exception );
    }

    return input;
}


/**
 * @builtin OpenDialog
 * @id OpenDialog_with_options
 * @short Opens a Dialog with options
 * @description
 * Same as the OpenDialog with one argument, but you can specify options
 * with a term of the form <tt><b>`opt</b></tt>.
 *
 * The <tt>`mainDialog</tt> option creates a "main window" dialog:
 * The dialog will get a large "default size". In the Qt UI, this typically
 * means 800x600 pixels large (or using a -geometry command line argument if
 * present) or full screen. In the NCurses UI, this is always full screen.
 *
 * <tt>`defaultsize</tt> is an alias for <tt>`mainDialog</tt>.
 *
 * <tt>`wizardDialog</tt> is a main dialog that will contain a wizard widget.
 * For UIs that don't support this kind of specialized dialog, this is
 * equivalent to <tt>`mainDialog</tt> -- see also the
 * <tt>HasWizardDialogSupport</tt> entry of the map returned by
 * <tt>UI::GetDisplayInfo()</tt>. 
 *
 * The <tt>`warncolor</tt> option displays the entire dialog in a bright
 * warning color.
 *
 * The <tt>`infocolor</tt> option displays the dialog in a color scheme that is
 * distinct from the normal colors, but not as bright as warncolor.
 *
 * The <tt>`decorated</tt> option is now obsolete, but still accepted to keep
 * old code working.
 *
 * The <tt>`centered</tt> option is now obsolete, but still accepted to keep
 * old code working.
 *
 * @param term options
 * @param term widget
 * @return boolean true if success, false if error
 *
 * @usage OpenDialog( `opt( `defaultsize ), `Label( "Hello, World!" ) )
 */

YCPBoolean YCP_UI::OpenDialog( const YCPTerm & opts, const YCPTerm & dialogTerm )
{
    YDialogType		dialogType = YPopupDialog;
    YDialogColorMode	colorMode  = YDialogNormalColor;

    if ( ! opts.isNull() ) // evaluate `opt() contents
    {
	    YCPList optList = opts->args();

	    for ( int o=0; o < optList->size(); o++ )
	    {
		if ( optList->value(o)->isSymbol() )
		{
		    if      ( optList->value(o)->asSymbol()->symbol() == YUIOpt_mainDialog  ) 	dialogType = YMainDialog;
		    else if ( optList->value(o)->asSymbol()->symbol() == YUIOpt_defaultsize ) 	dialogType = YMainDialog;
		    else if ( optList->value(o)->asSymbol()->symbol() == YUIOpt_wizardDialog )	dialogType = YWizardDialog;
		    else if ( optList->value(o)->asSymbol()->symbol() == YUIOpt_infocolor )	colorMode  = YDialogInfoColor;
		    else if ( optList->value(o)->asSymbol()->symbol() == YUIOpt_warncolor )	colorMode  = YDialogWarnColor;
		    else if ( optList->value(o)->asSymbol()->symbol() == YUIOpt_decorated ) 	{} // obsolete
		    else if ( optList->value(o)->asSymbol()->symbol() == YUIOpt_centered  )	{} // obsolete

		    else
			yuiWarning() << "Unknown option " << opts->value(o) << " for OpenDialog" << endl;
		}
	    }
    }

    YUI::ui()->blockEvents();	// Prevent self-generated events from UI built-ins.

    bool ok = true;

    try
    {
	if ( dialogType == YWizardDialog && ! YUI::yApp()->hasWizardDialogSupport() )
	    dialogType = YMainDialog;
	
	YDialog * dialog = YUI::widgetFactory()->createDialog( dialogType, colorMode );
	YUI_CHECK_NEW( dialog );

	YCPDialogParser::parseWidgetTreeTerm( dialog, dialogTerm );
	dialog->open();
    }
    catch ( YUIException & exception )
    {
	YUI_CAUGHT( exception );

	// Delete this half-created dialog:
	// Some widgets are in a very undefined state (no children etc.)
	YDialog::deleteTopmostDialog();

	ycperror( "UI::OpenDialog() failed" );
	ok = false;

	YCPErrorDialog::exceptionDialog( "UI Syntax Error", exception );
    }

    YUI::ui()->unblockEvents();

    return YCPBoolean( ok );
}



/**
 * @builtin CloseDialog()
 * @short Closes an open dialog
 * @description
 * Closes the most recently opened dialog. It is an error
 * to call <tt>CloseDialog</tt> if no dialog is open.
 *
 * @return boolean Returns true on success.
 */

YCPValue YCP_UI::CloseDialog()
{
    YUI::ui()->blockEvents();	// We don't want self-generated events from UI builtins.
    YDialog::deleteTopmostDialog();
    YUI::ui()->unblockEvents();

    return YCPBoolean( true );
}



/**
 * @builtin ChangeWidget
 * @short Changes widget contents
 * @description
 * Changes a property of a widget of the topmost dialog. <tt>id</tt> specified
 * the widget to change, <tt>property</tt> specifies the property that should
 * be changed, <tt>newvalue</tt> gives the new value.
 *
 * For example in order to change the label of an InputField with id `name to
 * "anything", you write <tt>ChangeWidget( `id(`name), `Label, "anything" )</tt>.
 * @param symbol widgetId Can also be specified as `id( any widgetId )
 * @param symbol property
 * @param any newValue
 *
 * @return boolean Returns true on success.
 */

YCPValue YCP_UI::ChangeWidget( const YCPValue & idValue, const YCPValue & property, const YCPValue & newValue )
{
    YCPValue ret = YCPVoid();

    try
    {
	YUI::ui()->blockEvents();	// We don't want self-generated events from UI::ChangeWidget().

	if ( ! YCPDialogParser::isSymbolOrId( idValue ) )
	{
	    YUI_THROW( YUISyntaxErrorException( string( "Expected `id(...) or `symbol, not " ) +
						idValue->toString().c_str() ) );
	}

	YCPValue	id	= YCPDialogParser::parseIdTerm( idValue );
	YWidget *	widget 	= YCPDialogParser::findWidgetWithId( id,
								     true ); // throw if not found

	YPropertySet propSet = widget->propertySet();

	if ( property->isSymbol() )
	{
	    string oldShortcutString = widget->shortcutString();
	    string propertyName	 = property->asSymbol()->symbol();

	    YPropertyValue val;

	    if		( newValue->isString()  )	val = YPropertyValue( newValue->asString()->value()  );
	    else if	( newValue->isInteger() )	val = YPropertyValue( newValue->asInteger()->value() );
	    else if	( newValue->isBoolean() )	val = YPropertyValue( newValue->asBoolean()->value() );
	    else
		val = YPropertyValue( false ); // Dummy value, will be rejected anyway

	    bool success = widget->setProperty( propertyName, val );

	    if ( ! success )
	    {
		// Try again with the known special cases
		success = YCPPropertyHandler::setComplexProperty( widget, propertyName, newValue );
	    }

	    ret = YCPBoolean( success );

	    if ( oldShortcutString != widget->shortcutString() )
		YDialog::currentDialog()->checkShortcuts();
	}
	else if ( property->isTerm() )
	{
	    bool success	= YCPPropertyHandler::setComplexProperty( widget, property->asTerm(), newValue );
	    ret		= YCPBoolean( success );
	}
	else
	{
	    YUI_THROW( YUISyntaxErrorException( string( "Bad UI::ChangeWidget args: " )
						+ property->toString() ) );
	}
    }
    catch( YUIException & exception )
    {
	YUI_CAUGHT( exception );
	ycperror( "UI::ChangeWidget failed: UI::ChangeWidget( %s, %s, %s )",
		  idValue->toString().c_str(),
		  property->toString().c_str(),
		  newValue->toString().c_str() );
	ret = YCPNull();
    }

    YUI::ui()->unblockEvents();

    return ret;
}



/**
 * @builtin QueryWidget
 * @short Queries Widget contents
 * @description
 * Queries a property of a widget of the topmost dialog.  For example in order
 * to query the current text of an InputField with id `name, you write
 * <tt>QueryWidget( `id(`name), `Value )</tt>. In some cases the propery can be given
 * as term in order to further specify it. An example is
 * <tt>QueryWidget( `id( `table ), `Item( 17 ) )</tt> for a table where you query a
 * certain item.
 *
 * @param  symbol widgetId Can also be specified as `id( any id )
 * @param symbol|term property
 * @return any
 */

YCPValue YCP_UI::QueryWidget( const YCPValue & idValue, const YCPValue & property )
{
    YCPValue ret = YCPVoid();

    try
    {
	if ( ! YCPDialogParser::isSymbolOrId( idValue ) )
	{
	    YUI_THROW( YUISyntaxErrorException( string( "Expected `id(...) or `symbol, not " ) +
						idValue->toString().c_str() ) );
	}

	YCPValue id = YCPDialogParser::parseIdTerm( idValue );
	YWidget *widget = YCPDialogParser::findWidgetWithId( id,
							     true ); // throw if not found

	YPropertySet propSet = widget->propertySet();

	if ( property->isSymbol() )		// The normal case: UI::QueryWidget(`myWidget, `SomeProperty)
	{
	    string propertyName = property->asSymbol()->symbol();
	    YPropertyValue val  = widget->getProperty( propertyName );

	    switch ( val.type() )
	    {
		case YStringProperty:	return YCPString ( val.stringVal()  );
		case YBoolProperty:	return YCPBoolean( val.boolVal()    );
		case YIntegerProperty:	return YCPInteger( val.integerVal() );
		case YOtherProperty:	return YCPPropertyHandler::getComplexProperty( widget, propertyName );

		default:
		    ycperror( "Unknown result for setProperty( %s )", propertyName.c_str() );
		    return YCPVoid();
	    }
	}
	else if ( property->isTerm() )	// Very rare: UI::QueryWidget(`myTable, `Item("abc", 3) )
	{
	    return YCPPropertyHandler::getComplexProperty( widget, property->asTerm() );
	}
	else
	{
	    YUI_THROW( YUISyntaxErrorException( string( "Bad UI::QueryWidget args: " )
						+ property->toString() ) );
	}
    }
    catch( YUIException & exception )
    {
	YUI_CAUGHT( exception );
	ycperror( "UI::QueryWidget failed: UI::QueryWidget( %s, %s )",
		  idValue->toString().c_str(),
		  property->toString().c_str() );
	ret = YCPNull();
    }

    return ret;
}


/**
 * @builtin ReplaceWidget
 *
 * @description
 * Replaces a complete widget (or widget subtree) with an other widget
 * (or widget tree). You can only replace the widget contained in
 * a <tt>ReplacePoint</tt>. As parameters to <tt>ReplaceWidget</tt>
 * specify the id of the ReplacePoint and the new widget.
 *
 * @param symbol id
 * @param term newWidget
 * @return true if success, false if failed
 */

YCPBoolean YCP_UI::ReplaceWidget( const YCPValue & idValue, const YCPTerm & newContentTerm )
{
    bool success = true;

    try
    {
	if ( ! YCPDialogParser::isSymbolOrId( idValue ) )
	{
	    YUI_THROW( YUISyntaxErrorException( string( "Expected `id(...) or `symbol, not " ) +
						idValue->toString().c_str() ) );
	}

	YUI::ui()->blockEvents();	// Prevent self-generated events
	YCPValue  id     = YCPDialogParser::parseIdTerm( idValue );
	YWidget * widget = YCPDialogParser::findWidgetWithId( id,
							      true ); // throw if not found
	if ( ! widget ) return YCPBoolean( false );

	YReplacePoint * replacePoint = dynamic_cast<YReplacePoint *> (widget);

	if ( ! replacePoint )
	    YUI_THROW( YUIException( string( "Widget with ID " ) + id->toString() + " is not a ReplacePoint" ) );

#if VERBOSE_REPLACE_WIDGET
	replacePoint->dumpDialogWidgetTree();
#endif
	YDialog * dialog = YDialog::currentDialog();

	YWidget::OptimizeChanges below( *dialog ); // delay screen updates until this block is left
	replacePoint->deleteChildren();

	YCPDialogParser::parseWidgetTreeTerm( replacePoint, newContentTerm );
	replacePoint->showChild();

#if VERBOSE_REPLACE_WIDGET
	replacePoint->dumpDialogWidgetTree();
#endif

	dialog->setInitialSize();
	dialog->checkShortcuts();
    }
    catch( YUIException & exception )
    {
	YUI_CAUGHT( exception );
	success = false;

	ycperror( "UI::ReplaceWidget() failed: UI::ReplaceWidget( %s, %s )",
		  idValue->toString().c_str(),
		  newContentTerm->toString().c_str() );

	YCPErrorDialog::exceptionDialog( "UI Syntax Error", exception );
    }

    YUI::ui()->unblockEvents();

    return YCPBoolean( success );
}



/**
 * @builtin WizardCommand
 * @short Runs a wizard command
 * @description
 * Issues a command to a wizard widget with ID 'wizardId'.
 *
 * <b>This builtin is not for general use. Use the Wizard.ycp module instead.</b>
 *
 * For available wizard commands see file YCPWizardCommandParser.cc .
 * If the current UI does not provide a wizard widget, 'false' is returned.
 * It is safe to call this even for UIs that don't provide a wizard widget. In
 * this case, all calls to this builtin simply return 'false'.
 *
 * @param term wizardCommand
 *
 * @return boolean  Returns true on success.
 */

YCPValue YCP_UI::WizardCommand( const YCPTerm & command )
{
    if ( ! YUI::optionalWidgetFactory()->hasWizard() )
	return YCPBoolean( false );

    // A wizard widget always has ID `wizard
    YWidget * widget = YCPDialogParser::findWidgetWithId( YCPSymbol( YWizardID ),
							  false ); // don't throw if not found

    if ( ! widget )
	return YCPBoolean( false );

    YWizard * wizard = dynamic_cast<YWizard *>( widget );

    if ( ! wizard )
	return YCPBoolean( false );

    YUI::ui()->blockEvents();	// Avoid self-generated events from builtins
    bool ret = YCPWizardCommandParser::parseAndExecute( wizard, command );
    YUI::ui()->unblockEvents();

    return YCPBoolean( ret );
}



/**
 * @builtin SetFocus
 * @short Sets Focus to the specified widget
 * @description
 * Sets the keyboard focus to the specified widget.  Notice that not all
 * widgets can accept the keyboard focus; this is limited to interactive
 * widgets like PushButtton, InputField, SelectionBox etc. - manager widgets
 * like VBox, HBox etc. will not accept the keyboard focus. They will not
 * propagate the keyboard focus to some child widget that accepts the
 * focus. Instead, an error message will be emitted into the log file.
 * @param symbol widgetId
 * @return boolean Returns true on success (i.e. the widget accepted the focus).
 */

YCPBoolean YCP_UI::SetFocus( const YCPValue & idValue )
{
    if ( ! YCPDialogParser::isSymbolOrId( idValue ) )
	return YCPNull();

    YCPBoolean result = YCPNull();
    
    try
    {
	YCPValue id = YCPDialogParser::parseIdTerm( idValue );
	YWidget * widget = YCPDialogParser::findWidgetWithId( id );

	if ( ! widget )
	    return YCPBoolean( false );

	result = YCPBoolean( widget->setKeyboardFocus() );
    }
    catch ( YUIException & exception )
    {
	YUI_CAUGHT( exception );
	ycperror( "UI::SetFocus() failed" );
    }

    return result;
}



/**
 * @builtin BusyCursor
 * @short Sets the mouse cursor to the busy cursor
 * @description
 * Sets the mouse cursor to the busy cursor, if the UI supports such a feature.
 *
 * This should normally not be necessary. The UI handles mouse cursors itself:
 * When input is possible (i.e. inside UserInput() ), there is automatically a
 * normal cursor, otherwise, there is the busy cursor. Override this at your
 * own risk.
 *
 * @return void
 */

void YCP_UI::BusyCursor()
{
    YUI::app()->busyCursor();
}


/**
 * @builtin NormalCursor
 * @short Sets the mouse cursor to the normal cursor
 * @description
 * Sets the mouse cursor to the normal cursor (after BusyCursor), if the UI
 * supports such a feature.
 *
 * This should normally not be necessary. The UI handles mouse cursors itself:
 * When input is possible (i.e. inside UserInput() ), there is automatically a
 * normal cursor, otherwise, there is the busy cursor. Override this at your
 * own risk.
 *
 * @return void
 */

void YCP_UI::NormalCursor()
{
    YUI::app()->normalCursor();
}


/**
 * @builtin TextMode
 * @short Check if the UI is running in text (NCurses) mode.
 *
 * @description
 *
 * This checks if a text mode UI (NCurses) is currently running.
 * Please notice that this is almost always bad style. The purpose of the YaST2
 * UI is to have an abstraction layer to avoid this kind of check.
 *
 * When you find yourself using this built-in, please check if there is really
 * no other way. In particular, NEVER rely on this check to make any
 * assumptions on the UI's capabilities. Rather, use the map entries from
 * UI::GetDisplayInfo() or UI::HasSpecialWidget().
 *
 * If you feel you must make a difference between text mode and GUI mode for a
 * dialog's layout, please check if that dialog is not simply overcrowded and
 * thus should be redesigned - it is very likely that it is also overly
 * complex.
 *
 * Also keep in mind that layout units are UI independent; a spacing should
 * consume about the same amount of space in all UIs. A Qt UI 800x600 main
 * window corresponds to 80x25 layout units, i.e. the typical NCurses terminal
 * size.
 *
 * @return true if text mode, false if GUI (Qt / Gtk).
 */

YCPBoolean YCP_UI::TextMode()
{
    ycpwarning( "UI::TextMode(): Please check if this is really necessary!" );

    return YCPBoolean( YUI::app()->isTextMode() );
}


/**
 * @builtin RedrawScreen
 * @short Redraws the screen
 * @description
 * Redraws the screen after it very likely has become garbled by some other output.
 *
 * This should normally not be necessary: The (specific) UI redraws the screen
 * automatically whenever required. Under rare circumstances, however, the
 * screen might have changes due to circumstances beyond the UI's control: For
 * text based UIs, for example, system commands that cause output to every tty
 * might make this necessary. Call this in the YCP code after such a command.
 *
 * @return void
 */

void YCP_UI::RedrawScreen()
{
    YUI::app()->redrawScreen();
}



/**
 * @builtin MakeScreenShot
 * @short Makes Screen Shot
 * @description
 * Makes a screen shot if the specific UI supports that.
 * The Qt UI opens a file selection box if filename is empty.
 *
 * @param string filename
 * @return void
 */

void YCP_UI::MakeScreenShot( const YCPString & filename )
{
    YUI::app()->makeScreenShot( filename->value () );
}



/**
 * @builtin DumpWidgetTree
 * @short Debugging function
 * @description
 * Debugging function: Dumps the widget tree of the current dialog to the log
 * file.
 *
 * @return void
 */

void YCP_UI::DumpWidgetTree()
{
    // Do not abort() the whole thing just because some sloppy
    // YCP client does not know how to close dialogs (#483942)
    // After all, this is a debugging function
    YDialog *currentDialog = YDialog::currentDialog(false);

    if (currentDialog) 
        currentDialog->dumpDialogWidgetTree();
    else
	yuiWarning() << "No dialog exists :( Nothing to dump." << endl;
}

/**
 * @builtin Beep
 * @short Beeps the system bell
 * @description
 * Beeps the system bell. This is implemented by the frontend, which may do
 * a visual beep if the system is set up that way (eg. for accessiblity
 * purposes).
 *
 * @return void
 */
void YCP_UI::Beep()
{
    YUI::app()->beep();
}


/**
 * @builtin RecordMacro
 * @short Records Macro into a file
 * @description
 * Begins recording a macro. Write the macro contents to file "macroFilename".
 * @param string macroFileName
 * @return void
 */
void YCP_UI::RecordMacro( const YCPString & filename )
{
    YMacro::record( filename->value () );
}



/**
 * @builtin StopRecordingMacro
 * @short Stops recording macro
 * @description
 * Stops macro recording. This is only necessary if you don't wish to record
 * everything until the program terminates.
 *
 * @return void
 */
void YCP_UI::StopRecordMacro()
{
    YMacro::endRecording();
}


/**
 * @builtin PlayMacro
 * @short Plays a recorded macro
 * @description
 * Executes everything in macro file "macroFileName".
 * Any errors are sent to the log file only.
 * The macro can be terminated only from within the macro file.
 *
 * @param string macroFileName
 * @return void
 */
void YCP_UI::PlayMacro( const YCPString & filename )
{
    YMacro::play( filename->value() );
}


/**
 * @builtin FakeUserInput
 * @short Fakes User Input
 * @description
 * Prepares a fake value for the next call to UserInput() -
 * i.e. the next UserInput() will return exactly this value.
 * This is only useful in connection with macros.
 *
 * If called without a parameter, the next call to UserInput()
 * will return "nil".
 *
 * @optarg any nextUserInput
 * @return void
 */
void YCP_UI::FakeUserInput( const YCPValue & next_input )
{
    yuiDebug() << "UI::FakeUserInput( " << next_input << " )" << endl;
    _fakeUserInputQueue.push_back( next_input );
}



/**
 * @builtin Glyph
 * @short Returns a special character (a 'glyph')
 * @description
 * Returns a special character (a 'glyph') according to the symbol specified.
 *
 * Not all UIs may be capable of displaying every glyph; if a specific UI
 * doesn't support it, a textual representation (probably in plain ASCII) will
 * be returned.
 *
 * This is also why there is only a limited number of predefined
 * glyphs: An ASCII equivalent is required which is sometimes hard to find for
 * some characters defined in Unicode / UTF-8.
 *
 * Please note the value returned may consist of more than one character; for
 * example, Glyph( `ArrowRight ) may return something like "-&gt;".
 *
 * If an unknown glyph symbol is specified, 'nil' is returned.
 *
 * @param symbol glyph
 * @return string
 */
YCPString YCP_UI::Glyph( const YCPSymbol & glyphSym )
{
    return YCPString( YUI::app()->glyph( glyphSym->symbol() ) );
}


/**
 * @builtin GetDisplayInfo
 * @short Gets Display Info
 * @description
 * Gets information about the current display and the UI's capabilities.
 *
 * Function output might differ according to the system where called.
 *
 * @return map <string any>
 *
 */
YCPMap YCP_UI::GetDisplayInfo()
{
    YCPMap info_map;
    YApplication * app = YUI::app(); // slight optimization

    info_map->add( YCPString( YUICap_Width			), YCPInteger( app->displayWidth()	) );
    info_map->add( YCPString( YUICap_Height			), YCPInteger( app->displayHeight()	) );
    info_map->add( YCPString( YUICap_Depth			), YCPInteger( app->displayDepth()	) );
    info_map->add( YCPString( YUICap_Colors			), YCPInteger( app->displayColors()	) );
    info_map->add( YCPString( YUICap_DefaultWidth		), YCPInteger( app->defaultWidth()	) );
    info_map->add( YCPString( YUICap_DefaultHeight		), YCPInteger( app->defaultHeight()	) );
    info_map->add( YCPString( YUICap_TextMode			), YCPBoolean( app->isTextMode()	) );
    info_map->add( YCPString( YUICap_HasImageSupport		), YCPBoolean( app->hasImageSupport()	) );
    info_map->add( YCPString( YUICap_HasIconSupport		), YCPBoolean( app->hasIconSupport()	) );
    info_map->add( YCPString( YUICap_HasAnimationSupport	), YCPBoolean( app->hasAnimationSupport()   ) );
    info_map->add( YCPString( YUICap_HasFullUtf8Support		), YCPBoolean( app->hasFullUtf8Support()    ) );
    info_map->add( YCPString( YUICap_RichTextSupportsTable	), YCPBoolean( app->richTextSupportsTable() ) );
    info_map->add( YCPString( YUICap_LeftHandedMouse		), YCPBoolean( app->leftHandedMouse()	) );
    info_map->add( YCPString( YUICap_y2debug			), YCPBoolean( YUILog::debugLoggingEnabled() ) );

    return info_map;
}


/**
 * @builtin RecalcLayout
 * @short Recalculates Layout
 * @description
 * Recompute the layout of the current dialog.
 *
 * <b>This is a very expensive operation.</b>
 *
 * Use this after changing widget properties that might affect their size -
 * like the a Label widget's value. Call this once (!) after changing all such
 * widget properties.
 *
 * @return void
 */
void YCP_UI::RecalcLayout()
{
    YDialog::currentDialog()->setInitialSize();
}


/**
 * @builtin PostponeShortcutCheck
 * @short Postpones Shortcut Check
 * @description
 * Postpone keyboard shortcut checking during multiple changes to a dialog.
 *
 * Normally, keyboard shortcuts are checked automatically when a dialog is
 * created or changed. This can lead to confusion, however, when multiple
 * changes to a dialog (repeated ReplaceWidget() calls) cause unwanted
 * intermediate states that may result in shortcut conflicts while the dialog
 * is not final yet. Use this function to postpone this checking until all
 * changes to the dialog are done and then explicitly check with
 * <tt>CheckShortcuts()</tt>. Do this before the next call to
 * <tt>UserInput()</tt> or <tt>PollInput()</tt> to make sure the dialog doesn't
 * change "on the fly" while the user tries to use one of those shortcuts.
 *
 * The next call to <tt>UserInput()</tt> or <tt>PollInput()</tt> will
 * automatically perform that check if it hasn't happened yet, any an error
 * will be issued into the log file.
 *
 * Use only when really necessary. The automatic should do well in most cases.
 *
 * The normal sequence looks like this:
 *
 * <code>
 * PostponeShortcutChecks();
 * ReplaceWidget( ... );
 * ReplaceWidget( ... );
 * ...
 * ReplaceWidget( ... );
 * CheckShortcuts();
 * ...
 * UserInput();
 * </code>
 *
 * @return void
 */
void YCP_UI::PostponeShortcutCheck()
{
    YDialog::currentDialog()->postponeShortcutCheck();
}


/**
 * @builtin CheckShortcuts
 * @short Performs an explicit shortcut check after postponing shortcut checks.
 * @description
 * Performs an explicit shortcut check after postponing shortcut checks.
 * Use this after calling <tt>PostponeShortcutCheck()</tt>.
 *
 * The normal sequence looks like this:
 *
 * <code>
 * PostponeShortcutChecks();
 * ReplaceWidget( ... );
 * ReplaceWidget( ... );
 * ...
 * ReplaceWidget( ... );
 * CheckShortcuts();
 * ...
 * UserInput();
 * </code>
 *
 * @return void
 */
void YCP_UI::CheckShortcuts()
{
    YDialog * dialog = YDialog::currentDialog();

    if ( ! dialog->shortcutCheckPostponed() )
    {
	yuiWarning() << "Use UI::CheckShortcuts() only after UI::PostponeShortcutCheck() !" << endl;
    }

    dialog->checkShortcuts( true );
}


/**
 * @builtin WidgetExists
 * @short Checks whether or not a widget with the given ID currently exists
 * @description
 * Checks whether or not a widget with the given ID currently exists in the
 * current dialog. Use this to avoid errors in the log file before changing the
 * properties of widgets that might or might not be there.
 *
 * @param symbol widgetId
 * @return boolean
 */
YCPBoolean YCP_UI::WidgetExists( const YCPValue & idValue )
{
    if ( ! YCPDialogParser::isSymbolOrId( idValue ) ) return YCPNull();

    YCPValue id = YCPDialogParser::parseIdTerm( idValue );
    YWidget *widget = YCPDialogParser::findWidgetWithId( id,
							 false ); // Don't throw if not found
    return widget ? YCPBoolean( true ) : YCPBoolean( false );
}


/**
 * @builtin RunPkgSelection
 * @short Initializes and run the PackageSelector widget
 * @description
 * <b>Not to be used outside the package selection</b>
 *
 * Initialize and run the PackageSelector widget identified by 'pkgSelId'.
 *
 * Black magic to everybody outside. ;- )
 *
 * @param any pkgSelId
 * @return any Returns `cancel if the user wishes to cancel his selections.
 *
 */
YCPValue YCP_UI::RunPkgSelection( const YCPValue & value_id )
{
    YCPValue result = YCPNull();

    try
    {
	if ( ! YCPDialogParser::isSymbolOrId( value_id ) )
	{
	    yuiError() << "RunPkgSelection(): expecting `id( ... ), not " << value_id << endl;
	    return YCPNull();
	}

	YCPValue id = YCPDialogParser::parseIdTerm( value_id );
	YWidget * selector = YCPDialogParser::findWidgetWithId( id );

	yuiMilestone() << "Running package selection..." << endl;
	YEvent * event = YUI::ui()->runPkgSelection( selector );

	if ( event )
	{
	    YCPEvent ycpEvent( event );
	    result = ycpEvent.eventId();

	    if ( result->isString() )
		result = YCPSymbol( result->asString()->value() ); // "accept" -> `accept

	    yuiMilestone() << "Package selection done. Returning with " << result << endl;
	}
    }
    catch ( YUIException & exception )
    {
	YUI_CAUGHT( exception );
	ycperror( "RunPkgSelection() failed" );
	YDialog::currentDialog()->dumpWidgetTree();
    }

    return result;
}



/**
 * @builtin AskForExistingDirectory
 * @short Ask user for existing directory
 * @description
 * Opens a directory selection box and prompt the user for an existing directory.
 *
 * @param string startDir is the initial directory that is displayed.
 * @param string headline is an explanatory text for the directory selection box.
 * Graphical UIs may omit that if no window manager is running.
 * @return string  Returns the selected directory name or <i>nil</i> if the
 * user canceled the operation.
 */
YCPValue YCP_UI::AskForExistingDirectory( const YCPString & startDir, const YCPString & headline )
{
    string ret = YUI::app()->askForExistingDirectory( startDir->value(), headline->value() );

    if ( ret.empty() )
	return YCPVoid();
    else
	return YCPString( ret );
}



/**
 * @builtin AskForExistingFile
 * @short Ask user for existing file
 * @description
 * Opens a file selection box and prompt the user for an existing file.
 *
 * @param string startWith is the initial directory or file.
 * @param string filter is one or more blank-separated file patterns, e.g. "*.png *.jpg"
 * @param string headline is an explanatory text for the file selection box.
 * Graphical UIs may omit that if no window manager is running.
 * @return string Returns the selected file name or <i>nil</i> if the user
 * canceled the operation.
 */
YCPValue YCP_UI::AskForExistingFile( const YCPString & startWith,
				     const YCPString & filter,
				     const YCPString & headline )
{
    string ret = YUI::app()->askForExistingFile( startWith->value(), filter->value(), headline->value() );

    if ( ret.empty() )
	return YCPVoid();
    else
	return YCPString( ret );
}


/**
 * @builtin AskForSaveFileName
 * @short Ask user for a file to save data to.
 * @description
 * Opens a file selection box and prompt the user for a file to save data to.
 * Automatically asks for confirmation if the user selects an existing file.
 *
 * @param string startWith is the initial directory or file.
 * @param string filter is one or more blank-separated file patterns, e.g. "*.png *.jpg"
 * @param string headline is an explanatory text for the file selection box.
 * Graphical UIs may omit that if no window manager is running.
 *
 * @return string Returns the selected file name or <i>nil</i> if the user canceled the operation.
 */
YCPValue YCP_UI::AskForSaveFileName( const YCPString & startWith,
				     const YCPString & filter,
				     const YCPString & headline )
{
    string ret = YUI::app()->askForSaveFileName( startWith->value(), filter->value(), headline->value() );

    if ( ret.empty() )
	return YCPVoid();
    else
	return YCPString( ret );
}

/**
 * @builtin SetFunctionKeys
 * @short Sets the (default) function keys for a number of buttons.
 * @description
 * This function receives a map with button labels and the respective function
 * key number that should be used if on other `opt( `key_F.. ) is specified.
 *
 * Any keyboard shortcuts in those labels are silently ignored so this is safe
 * to use even if the UI's internal shortcut manager rearranges shortcuts.
 *
 * Each call to this function overwrites the data of any previous calls.
 *
 * @param map fkeys
 * @return void
 * @usage SetFunctionKeys( $[ "Back": 8, "Next": 10, ... ] );
 */
void YCP_UI::SetFunctionKeys( const YCPMap & new_fkeys )
{
    for ( YCPMapIterator it = new_fkeys->begin(); it != new_fkeys->end(); ++it )
    {
	if ( it.key()->isString() && it.value()->isInteger() )
	{
	    string label = YShortcut::cleanShortcutString( it.key()->asString()->value() );
	    int	   fkey  = it.value()->asInteger()->value();

	    if ( fkey > 0 && fkey <= 24 )
	    {
		yuiDebug() << "Mapping \"" << label << "\"\t-> F" << fkey << endl;
		YUI::app()->setDefaultFunctionKey( label, fkey );
	    }
	    else
	    {
		ycperror( "SetFunctionKeys(): Function key %d out of range for \"%s\"",
			  fkey, label.c_str() );
	    }
	}
	else
	{
	    ycperror( "SetFunctionKeys(): Invalid map element: "
		      "Expected <string>: <integer>, not %s: %s",
		      it.key()->toString().c_str(), it.value()->toString().c_str() );
	}
    }
}


/**
 * @builtin Recode
 * @short Recodes encoding of string from or to "UTF-8" encoding.
 * @description
 * Recodes encoding of string from or to "UTF-8" encoding.
 * One of from/to must be "UTF-8", the other should be an
 * iso encoding specifier (i.e. "ISO-8859-1" for western languages,
 * "ISO-8859-2" for eastern languages, etc. )
 *
 * @param string fromEncoding
 * @param string toEncoding
 * @param string text
 * @return any
 */

YCPValue YCP_UI::Recode( const YCPString & fromEncoding,
			 const YCPString & toEncoding,
			 const YCPString & text )
{
    string outstr;
    if ( recode ( text->value(), fromEncoding->value(), toEncoding->value(), outstr ) != 0 )
    {
	static bool warned_about_recode = false;
	if ( ! warned_about_recode )
	{
	    yuiError() << "recode( " << fromEncoding << ", " << toEncoding << " )" << endl;
	    warned_about_recode = true;
	}
	// return text as-is
	return text;
    }
    return YCPString( outstr );
}


/**
 * @builtin WFM/SCR
 * @id WFM_SCR
 * @short callback
 * @description
 * This is used for a callback mechanism. The expression will
 * be sent to the WFM interpreter and evaluated there.
 * USE WITH CAUTION.
 *
 * @param block expression
 * @return any
 */
YCPValue YCP_UI::evaluateCallback( const YCPTerm & term, bool to_wfm )
{
    if ( term->size() != 1 )	// must have 1 arg - anything allowed
    {
	y2error( "No arguments" );
	return YCPNull();
    }

    Y2Component * callbackComponent = YUIComponent::uiComponent()->getCallback();

    if ( callbackComponent )
    {
	YCPValue ret = YCPNull();

	if ( to_wfm )	// if it goes to WFM, just send the value
	{
	    ret = callbackComponent->evaluate( term->value(0) );
	}
	else		// going to SCR, send the complete term
	{
	    ret = callbackComponent->evaluate( term );
	}
	return ret;
    }

    return YCPVoid();
}



/**
* @description
* Opens a context menu when the users right clickes a widget 
* 
* 
* Example: <tt>OpenContextMenu( `menu(
* [ `item(`id(`folder), "&Entry1"  ),
* `menu( "&Submenu1",
* [ `item(`id(`text),
* "&Entry2" ),
* `item(`id(`image),
* "&Entry3"     ) ]) ]  )); </tt>
*
* @param itemList list of menu items
* @return bool  Returns true when the context menu was shown, on error 
                (e.g. not supported by ui) false is returned.
                
*/
YCPBoolean YCP_UI::OpenContextMenu ( const YCPTerm & term )
{
    YCPList itemList = term->value(0)->asList();
 
    if ( YUI::app()->openContextMenu( YCPMenuItemParser::parseMenuItemList( itemList ) ) )
	return YCPBoolean( true );
    else
	return YCPBoolean( false );
}
 



// EOF

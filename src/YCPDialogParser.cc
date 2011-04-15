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

  File:		YUI_widgets.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include <string.h>		// strncasecmp()

#include <ycp/YCPString.h>
#include <ycp/YCPVoid.h>
#include <ycp/YCPInteger.h>
#include <ycp/YCPFloat.h>
#include <ycp/YCPMap.h>
#include <ycp/YCPBoolean.h>

#define y2log_component "ui"
#include <ycp/y2log.h>	// ycperror()

#define YUILogComponent "ui"
#include "YUILog.h"

#include "YCPDialogParser.h"

#include "YUI.h"
#include "YApplication.h"
#include "YCP_util.h"
#include "YUISymbols.h"
#include "YWidget.h"
#include "YCPValueWidgetID.h"
#include "YCPItemParser.h"
#include "YCPMenuItemParser.h"
#include "YCPTreeItemParser.h"
#include "YCPTableItemParser.h"
#include "YCP_UI_Exception.h"
#include "YDialog.h"
#include "YWidgetFactory.h"
#include "YOptionalWidgetFactory.h"
#include "YBothDim.h"
#include "YWidgetOpt.h"

#include "YAlignment.h"
#include "YBarGraph.h"
#include "YButtonBox.h"
#include "YCheckBox.h"
#include "YCheckBoxFrame.h"
#include "YComboBox.h"
#include "YDateField.h"
#include "YDownloadProgress.h"
#include "YDumbTab.h"
#include "YEmpty.h"
#include "YFrame.h"
#include "YImage.h"
#include "YInputField.h"
#include "YIntField.h"
#include "YLabel.h"
#include "YLayoutBox.h"
#include "YLogView.h"
#include "YMenuButton.h"
#include "YMultiLineEdit.h"
#include "YMultiProgressMeter.h"
#include "YMultiSelectionBox.h"
#include "YPackageSelector.h"
#include "YPartitionSplitter.h"
#include "YProgressBar.h"
#include "YPushButton.h"
#include "YRadioButton.h"
#include "YRadioButtonGroup.h"
#include "YReplacePoint.h"
#include "YRichText.h"
#include "YSelectionBox.h"
#include "YSlider.h"
#include "YSpacing.h"
#include "YSquash.h"
#include "YTable.h"
#include "YTimeField.h"
#include "YTree.h"
#include "YWizard.h"
#include "YTimezoneSelector.h"
#include "YGraph.h"
#include "YBusyIndicator.h"

using std::string;

/**
 * Complain in the log and throw syntax error exception.
 * Written as a macro to preserve the original C++ code location.
 **/
#define THROW_BAD_ARGS(ARG_TERM)			\
    do							\
    {							\
	string msg = "Invalid arguments for the ";	\
	msg += (ARG_TERM)->name();			\
	msg += " widget";				\
							\
	ycperror( "%s: %s", msg.c_str(),		\
		  (ARG_TERM)->toString().c_str() );	\
							\
	_YUI_THROW( YUISyntaxErrorException( msg ),	\
		    YUI_EXCEPTION_CODE_LOCATION );	\
    } while ( 0 )


YWidget *
YCPDialogParser::parseWidgetTreeTerm( YWidget *		p,
				      YWidgetOpt &	opt,
				      const YCPTerm &	term )
{
    YUI_CHECK_PTR( p );

    // Extract optional widget ID, if present
    int n;
    YCPValue id = getWidgetId( term, &n );


    // Extract optional widget options `opt( `xyz )

    YCPList rawopt = getWidgetOptions( term, &n );

    // Handle generic options

    /**
     * @widget	AAA_All-Widgets
     * @usage	---
     * @short	Generic options for all widgets
     * @class	YWidget
     *
     * @option	notify		Make UserInput() return on any action in this widget.
     *				Normally UserInput() returns only when a button is clicked;
     *				with this option on you can make it return for other events, too,
     *				e.g. when the user selects an item in a SelectionBox
     *				( if `opt( `notify ) is set for that SelectionBox ).
     *				Only widgets with this option set are affected.
     *
     * @option	notifyContextMenu Make this widget to send an event when the context menu is requested
     *				e.g. when the user clicks right mouse button
     *				( if `opt( `notifyContextMenu ) is set for that SelectionBox ).
     *				Only widgets with this option set are affected.
     *
     * @option	disabled	Set this widget insensitive, i.e. disable any user interaction.
     *				The widget will show this state by being greyed out
     *				(depending on the specific UI).
     *
     * @option	hstretch	Make this widget stretchable in the horizontal dimension.
     *
     * @option	vstretch	Make this widget stretchable in the vertical   dimension.
     *
     * @option	hvstretch	Make this widget stretchable in both dimensions.
     *
     * @option	autoShortcut	Automatically choose a keyboard shortcut for this widget and don't complain
     *				in the log file about the missing
     *				shortcut.
     *				Don't use this regularly for all widgets - manually chosen keyboard shortcuts
     *				are almost always better than those automatically assigned. Refer to the style guide
     *				for details.
     *				This option is intended used for automatically generated data, e.g., RadioButtons
     *				for software selections that come from file or from some other data base.
     *
     * @option	key_F1		(NCurses only) activate this widget with the F1 key
     * @option	key_F2		(NCurses only) activate this widget with the F2 key
     * @option	key_Fxx		(NCurses only) activate this widget with the Fxx key
     * @option	key_F24		(NCurses only) activate this widget with the F24 key
     * @option	key_none	(NCurses only) no function key for this widget
     *
     * @option	keyEvents	(NCurses only) Make UserInput() / WaitForEvent() return on keypresses within this widget.
     *				Exactly which keys trigger such a key event is UI specific.
     *				This is not for general use.
     *
     * @description
     *
     * This is not a widget for general usage, this is just a placeholder for
     * descriptions of options that all widgets have in common.
     *
     * Use them for any widget whenever it makes sense.
     *
     * @example AutoShortcut1.ycp AutoShortcut2.ycp
     **/

    YCPList ol;

    for ( int o=0; o<rawopt->size(); o++ )
    {
	if ( rawopt->value(o)->isSymbol() )
	{
	    string sym = rawopt->value(o)->asSymbol()->symbol();
	    if	    ( sym == YUIOpt_notify	) opt.notifyMode.setValue( true );
	    else if ( sym == YUIOpt_notifyContextMenu) opt.notifyContextMenu.setValue( true );
	    else if ( sym == YUIOpt_disabled	) opt.isDisabled.setValue( true );
	    else if ( sym == YUIOpt_hstretch	) opt.isHStretchable.setValue( true );
	    else if ( sym == YUIOpt_vstretch	) opt.isVStretchable.setValue( true );
	    else if ( sym == YUIOpt_hvstretch	) { opt.isHStretchable.setValue( true ); opt.isVStretchable.setValue( true ); }
	    else if ( sym == YUIOpt_autoShortcut ) opt.autoShortcut.setValue( true );
	    else if ( sym == YUIOpt_boldFont	) opt.boldFont.setValue( true );
	    else if ( sym == YUIOpt_keyEvents	) opt.keyEvents.setValue( true );
	    else if ( sym == YUIOpt_key_F1	) opt.key_Fxx.setValue(	 1 );
	    else if ( sym == YUIOpt_key_F2	) opt.key_Fxx.setValue(	 2 );
	    else if ( sym == YUIOpt_key_F3	) opt.key_Fxx.setValue(	 3 );
	    else if ( sym == YUIOpt_key_F4	) opt.key_Fxx.setValue(	 4 );
	    else if ( sym == YUIOpt_key_F5	) opt.key_Fxx.setValue(	 5 );
	    else if ( sym == YUIOpt_key_F6	) opt.key_Fxx.setValue(	 6 );
	    else if ( sym == YUIOpt_key_F7	) opt.key_Fxx.setValue(	 7 );
	    else if ( sym == YUIOpt_key_F8	) opt.key_Fxx.setValue(	 8 );
	    else if ( sym == YUIOpt_key_F9	) opt.key_Fxx.setValue(	 9 );
	    else if ( sym == YUIOpt_key_F10	) opt.key_Fxx.setValue( 10 );
	    else if ( sym == YUIOpt_key_F11	) opt.key_Fxx.setValue( 11 );
	    else if ( sym == YUIOpt_key_F12	) opt.key_Fxx.setValue( 12 );
	    else if ( sym == YUIOpt_key_F13	) opt.key_Fxx.setValue( 13 );
	    else if ( sym == YUIOpt_key_F14	) opt.key_Fxx.setValue( 14 );
	    else if ( sym == YUIOpt_key_F15	) opt.key_Fxx.setValue( 15 );
	    else if ( sym == YUIOpt_key_F16	) opt.key_Fxx.setValue( 16 );
	    else if ( sym == YUIOpt_key_F17	) opt.key_Fxx.setValue( 17 );
	    else if ( sym == YUIOpt_key_F18	) opt.key_Fxx.setValue( 18 );
	    else if ( sym == YUIOpt_key_F19	) opt.key_Fxx.setValue( 19 );
	    else if ( sym == YUIOpt_key_F20	) opt.key_Fxx.setValue( 20 );
	    else if ( sym == YUIOpt_key_F21	) opt.key_Fxx.setValue( 21 );
	    else if ( sym == YUIOpt_key_F22	) opt.key_Fxx.setValue( 22 );
	    else if ( sym == YUIOpt_key_F23	) opt.key_Fxx.setValue( 23 );
	    else if ( sym == YUIOpt_key_F24	) opt.key_Fxx.setValue( 24 );
	    else if ( sym == YUIOpt_key_none	) opt.key_Fxx.setValue( -1 );
	    else ol->add( rawopt->value(o) );
	}
	else if ( ! rawopt->value(o)->isTerm() )
	{
	    ycperror( "Invalid widget option %s. Options must be symbols or terms.",
		      rawopt->value(o)->toString().c_str() );
	}
	else ol->add( rawopt->value(o) );
    }


    //
    // Extract the widget class
    //

    YWidget * w	= 0;
    string    s	= term->name();

    if      ( s == YUIWidget_Bottom		)	w = parseAlignment		( p, opt, term, ol, n, YAlignUnchanged,	YAlignEnd	);
    else if ( s == YUIWidget_ButtonBox		)	w = parseButtonBox		( p, opt, term, ol, n );
    else if ( s == YUIWidget_CheckBox		)	w = parseCheckBox		( p, opt, term, ol, n );
    else if ( s == YUIWidget_CheckBoxFrame	)	w = parseCheckBoxFrame		( p, opt, term, ol, n );
    else if ( s == YUIWidget_ComboBox		)	w = parseComboBox		( p, opt, term, ol, n );
    else if ( s == YUIWidget_Empty		)	w = parseEmpty			( p, opt, term, ol, n );
    else if ( s == YUIWidget_Frame		)	w = parseFrame			( p, opt, term, ol, n );
    else if ( s == YUIWidget_HBox		)	w = parseLayoutBox		( p, opt, term, ol, n, YD_HORIZ );
    else if ( s == YUIWidget_HCenter		)	w = parseAlignment		( p, opt, term, ol, n, YAlignCenter, 	YAlignUnchanged );
    else if ( s == YUIWidget_HSpacing		)	w = parseSpacing		( p, opt, term, ol, n, YD_HORIZ, false );
    else if ( s == YUIWidget_HSquash		)	w = parseSquash			( p, opt, term, ol, n, true,  false );
    else if ( s == YUIWidget_HStretch		)	w = parseSpacing		( p, opt, term, ol, n, YD_HORIZ, true );
    else if ( s == YUIWidget_HVCenter		)	w = parseAlignment		( p, opt, term, ol, n, YAlignCenter,	YAlignCenter	);
    else if ( s == YUIWidget_HVSquash		)	w = parseSquash			( p, opt, term, ol, n, true,  true );
    else if ( s == YUIWidget_HWeight		)	w = parseWeight			( p, opt, term, ol, n, YD_HORIZ );
    else if ( s == YUIWidget_Heading		)	w = parseLabel			( p, opt, term, ol, n, true );
    else if ( s == YUIWidget_IconButton		)	w = parsePushButton		( p, opt, term, ol, n, true );
    else if ( s == YUIWidget_Image		)	w = parseImage			( p, opt, term, ol, n );
    else if ( s == YUIWidget_InputField		)	w = parseInputField		( p, opt, term, ol, n, false );
    else if ( s == YUIWidget_IntField		)	w = parseIntField		( p, opt, term, ol, n );
    else if ( s == YUIWidget_Label		)	w = parseLabel			( p, opt, term, ol, n, false );
    else if ( s == YUIWidget_Left		)	w = parseAlignment		( p, opt, term, ol, n, YAlignBegin,	YAlignUnchanged );
    else if ( s == YUIWidget_LogView		)	w = parseLogView		( p, opt, term, ol, n );
    else if ( s == YUIWidget_MarginBox		)	w = parseMarginBox		( p, opt, term, ol, n );
    else if ( s == YUIWidget_MenuButton		)	w = parseMenuButton		( p, opt, term, ol, n );
    else if ( s == YUIWidget_MinHeight		)	w = parseMinSize		( p, opt, term, ol, n, false, true  );
    else if ( s == YUIWidget_MinSize		)	w = parseMinSize		( p, opt, term, ol, n, true,  true  );
    else if ( s == YUIWidget_MinWidth		)	w = parseMinSize		( p, opt, term, ol, n, true,  false );
    else if ( s == YUIWidget_MultiLineEdit	)	w = parseMultiLineEdit		( p, opt, term, ol, n );
    else if ( s == YUIWidget_MultiSelectionBox	)	w = parseMultiSelectionBox	( p, opt, term, ol, n );
    else if ( s == YUIWidget_PackageSelector	)	w = parsePackageSelector	( p, opt, term, ol, n );
    else if ( s == YUIWidget_Password		)	w = parseInputField		( p, opt, term, ol, n, true );
    else if ( s == YUIWidget_PkgSpecial		)	w = parsePkgSpecial		( p, opt, term, ol, n );
    else if ( s == YUIWidget_ProgressBar	)	w = parseProgressBar		( p, opt, term, ol, n );
    else if ( s == YUIWidget_PushButton		)	w = parsePushButton		( p, opt, term, ol, n, false );
    else if ( s == YUIWidget_RadioButton	)	w = parseRadioButton		( p, opt, term, ol, n );
    else if ( s == YUIWidget_RadioButtonGroup	)	w = parseRadioButtonGroup	( p, opt, term, ol, n );
    else if ( s == YUIWidget_ReplacePoint	)	w = parseReplacePoint		( p, opt, term, ol, n );
    else if ( s == YUIWidget_RichText		)	w = parseRichText		( p, opt, term, ol, n );
    else if ( s == YUIWidget_Right		)	w = parseAlignment		( p, opt, term, ol, n, YAlignEnd,	YAlignUnchanged );
    else if ( s == YUIWidget_SelectionBox	)	w = parseSelectionBox		( p, opt, term, ol, n );
    else if ( s == YUIWidget_Table		)	w = parseTable			( p, opt, term, ol, n );
    else if ( s == YUIWidget_TextEntry		)	w = parseInputField		( p, opt, term, ol, n, false, true ); // bugCompatibilityMode
    else if ( s == YUIWidget_Top		)	w = parseAlignment		( p, opt, term, ol, n, YAlignUnchanged,	YAlignBegin	);
    else if ( s == YUIWidget_Tree		)	w = parseTree			( p, opt, term, ol, n );
    else if ( s == YUIWidget_VBox		)	w = parseLayoutBox		( p, opt, term, ol, n, YD_VERT );
    else if ( s == YUIWidget_VCenter		)	w = parseAlignment		( p, opt, term, ol, n, YAlignUnchanged,	YAlignCenter	);
    else if ( s == YUIWidget_VSpacing		)	w = parseSpacing		( p, opt, term, ol, n, YD_VERT, false );
    else if ( s == YUIWidget_VSquash		)	w = parseSquash			( p, opt, term, ol, n, false, true );
    else if ( s == YUIWidget_VStretch		)	w = parseSpacing		( p, opt, term, ol, n, YD_VERT, true );
    else if ( s == YUIWidget_VWeight		)	w = parseWeight			( p, opt, term, ol, n, YD_VERT );
    else if ( s == YUIWidget_BusyIndicator	)	w = parseBusyIndicator		( p, opt, term, ol, n );

    // Special widgets - may or may not be supported by the specific UI.
    // The YCP application should ask for presence of such a widget with Has???Widget() prior to creating one.

    else if ( s == YUISpecialWidget_Date		)	w = parseDateField		( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_DateField		)	w = parseDateField		( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_DummySpecialWidget	)	w = parseDummySpecialWidget	( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_DownloadProgress	)	w = parseDownloadProgress	( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_BarGraph		)	w = parseBarGraph		( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_DumbTab		)	w = parseDumbTab		( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_HMultiProgressMeter	)	w = parseMultiProgressMeter	( p, opt, term, ol, n, YD_HORIZ );
    else if ( s == YUISpecialWidget_VMultiProgressMeter	)	w = parseMultiProgressMeter	( p, opt, term, ol, n, YD_VERT  );
    else if ( s == YUISpecialWidget_PartitionSplitter	)	w = parsePartitionSplitter	( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_PatternSelector	)	w = parsePatternSelector	( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_SimplePatchSelector	)	w = parseSimplePatchSelector	( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_Slider		)	w = parseSlider			( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_Time		)	w = parseTimeField		( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_TimeField		)	w = parseTimeField		( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_Wizard		)	w = parseWizard			( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_TimezoneSelector	)	w = parseTimezoneSelector	( p, opt, term, ol, n );
    else if ( s == YUISpecialWidget_Graph		)	w = parseGraph			( p, opt, term, ol, n );
    else
    {
	YUI_THROW( YUIException( string( "Unknown widget type " ) + s.c_str() ) );
    }


    // Post-process the newly created widget

    if ( w )
    {
	if ( ! id.isNull()  &&	// ID specified for this widget
	     ! id->isVoid() )
	{
	    if ( ! w->hasId() )	// widget doesn't have an ID yet
		w->setId( new YCPValueWidgetID( id ) );

	    /*
	     * Note: Don't set the ID if it is already set!
	     * This is important for parseXy() functions that don't really create
	     * anything immediately but recursively call parseWidgetTreeTerm()
	     * internally - e.g. parseWeight(). In this case, the widget might
	     * already have an ID, so leave it alone.
	     *
	     * Otherwise, the ID that was specified for the Weight would be set
	     * on the Weight's child, which is not what the application
	     * programmer expects.
	     */
	}

	if ( opt.isDisabled.value() 	)	w->setDisabled();
	if ( opt.notifyMode.value() 	)	w->setNotify( true );
	if ( opt.notifyContextMenu.value())	w->setNotifyContextMenu( true );
	if ( opt.keyEvents.value()	)	w->setSendKeyEvents( true );
	if ( opt.autoShortcut.value()	)	w->setAutoShortcut( true );
	if ( opt.isHStretchable.value()	)	w->setStretchable( YD_HORIZ, true );
	if ( opt.isVStretchable.value()	)	w->setStretchable( YD_VERT,  true );
	if ( opt.key_Fxx.value() > 0	)
	{
	    YPushButton * button = dynamic_cast<YPushButton *> (w);
	    YButtonRole oldRole = button ? button->role() : YCustomButton;
	    w->setFunctionKey( opt.key_Fxx.value() );

	    if ( button && oldRole != button->role() && opt.customButton.value() )
	    {
		// Application requested button role override
		
		yuiMilestone() << "Overriding button role for " << button
			       << " to YCustomButton" << endl;
		button->setRole( YCustomButton );
	    }
	}
    }
    else
    {
	yuiError() << "Could not create " << s << endl;
	ycperror( "Could not create %s from\n%s", s.c_str(), term->toString().c_str() );
    }

    return w;
}


/**
 * Overloaded version - just for convenience.
 * Most callers don't need to set up the widget options before calling, so this
 * version will pass through an empty set of widget options.
 **/

YWidget *
YCPDialogParser::parseWidgetTreeTerm( YWidget * parent, const YCPTerm & term )
{
    YWidgetOpt opt;

    return parseWidgetTreeTerm( parent, opt, term );
}


// =============================================================================
//			       Mandatory Widgets
// =============================================================================


/**
 * @widget	ReplacePoint
 * @short	Pseudo widget to replace parts of a dialog
 * @class	YReplacePoint
 * @arg		term child the child widget
 * @usage	`ReplacePoint( `id( `rp ), `Empty() )
 * @example	ReplacePoint1.ycp DumbTab2.ycp ShortcutCheckPostponed.ycp WidgetExists.ycp
 *
 * @description
 *
 * A ReplacePoint can be used to dynamically change parts of a dialog.
 * It contains one widget. This widget can be replaced by another widget
 * by calling <tt>ReplaceWidget( `id( id ), newchild )</tt>, where <tt>id</tt> is the
 * the id of the new child widget of the replace point. The ReplacePoint widget
 * itself has no further effect and no optical representation.
 **/

YWidget *
YCPDialogParser::parseReplacePoint( YWidget * parent, YWidgetOpt & opt, const YCPTerm & term, const YCPList & optList,
				    int argnr )
{
    if ( term->size() != argnr+1 ||
	 term->value( argnr ).isNull() ||
	 ! term->value( argnr )->isTerm() )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    YReplacePoint * replacePoint = YUI::widgetFactory()->createReplacePoint( parent );
    parseWidgetTreeTerm( replacePoint, term->value( argnr )->asTerm() );
    replacePoint->showChild();

    return replacePoint;
}


/**
 * @widgets	Empty
 * @short	Placeholder widget
 * @class	YEmpty
 * @usage	`Empty()
 *
 * @description
 *
 * The Empty widget does nothing and has a default size of zero in both
 * dimensions. It is useful as a placeholder, for example as the initial child
 * of a <tt>ReplacePoint</tt>.
 **/

YWidget *
YCPDialogParser::parseEmpty( YWidget * parent, YWidgetOpt & opt,
			     const YCPTerm & term, const YCPList & optList, int argnr )
{
    if ( term->size() != argnr )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    return YUI::widgetFactory()->createEmpty( parent );
}



/**
 * @widgets	HSpacing VSpacing HStretch VStretch
 * @id          Spacing
 * @short	Fixed size empty space for layout
 * @class	YSpacing
 * @optarg	integer|float size
 * @usage	`HSpacing( 0.3 ) or `HStretch() or `HStretch( 0.7 )
 * @example	Spacing1.ycp Layout-Buttons-Equal-Even-Spaced2.ycp
 * 		HStretch1.ycp Layout-Buttons-Equal-Even-Spaced1.ycp
 *		Table2.ycp Table3.ycp
 *
 * @description
 *
 * HSpacing and VSpacing are layout helpers to add empty space between widgets.
 *
 * VStretch and HStretch act as "rubber bands" in layouts that take excess
 * space. They have a size of zero if there is no excess space.
 *
 * The <tt>size</tt> given is measured in units roughly equivalent to the size
 * of a character in the respective UI. Fractional numbers can be used here,
 * but text based UIs may choose to round the number as appropriate - even if
 * this means simply ignoring a spacing when its size becomes zero.
 *
 * If <tt>size</tt> is omitted, it defaults to 1.
 * <tt>HSpacing()</tt> will create a horizontal spacing with default width and zero height.
 * <tt>VSpacing()</tt> will create a vertical   spacing with default height and zero width.
 * <tt>HStretch()</tt> will create a horizontal stretch with zero width and height.
 * <tt>VStretch()</tt> will create a vertical   stretch with zero width and height.
 *
 * A HStretch or VStretch with a size specification will take at least the
 * specified amount of space, but it will take more (in that dimension) if
 * there is excess space in the layout.
 **/

YWidget *
YCPDialogParser::parseSpacing( YWidget * parent, YWidgetOpt & opt,
			       const YCPTerm & term, const YCPList & optList, int argnr,
			       YUIDimension dim, bool stretchable )
{
    float size	   = stretchable ? 0.0 : 1.0;
    bool  param_ok = false;

    if ( term->size() == argnr )		// no parameter
    {
	param_ok = true;
    }
    else if ( term->size() == argnr + 1 )	// one parameter
    {
	if ( term->value( argnr )->isInteger() )
	{
	    size	= (float) term->value( argnr )->asInteger()->value();
	    param_ok	= true;
	}
	else if ( term->value( argnr )->isFloat() )
	{
	    size	= term->value( argnr )->asFloat()->value();
	    param_ok	= true;
	}
    }

    if ( ! param_ok )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    return YUI::widgetFactory()->createSpacing( parent, dim, stretchable, size );
}



/**
 * @widgets	Left Right Top Bottom HCenter VCenter HVCenter
 * @id          Alignment
 * @short	Layout alignment
 * @class	YAlignment
 * @optarg	`BackgroundPixmap( "dir/pixmap.png" )	background pixmap
 * @arg		term child The contained child widget
 * @usage	`Left( `CheckBox( "Don't ask this again" ) )
 * @example	HCenter1.ycp HCenter2.ycp HCenter3.ycp Alignment1.ycp
 *
 * @description
 *
 * The Alignment widgets are used to control the layout of a dialog. They are
 * useful in situations, where to a widget is assigned more space than it can
 * use. For example if you have a VBox containing four CheckBoxes, the width of
 * the VBox is determined by the CheckBox with the longest label. The other
 * CheckBoxes are centered per default.
 *
 * With <tt>`Left( widget )</tt> you tell a
 * widget that it should be laid out leftmost of the space that is available to
 * it. <tt>Right, Top</tt> and <tt>Bottom</tt> are working accordingly.	 The
 * other three widgets center their child widget horizontally, vertically or in
 * both directions.
 *
 * As a very special case, alignment widgets that have `opt(`hvstretch) (and related)
 * set promote their child widget's stretchability to the parent layout.
 * I.e., they do not align a child that is stretchable in that dimension,
 * but stretch it to consume the available space. This is only very rarely
 * useful, such as in very generic layout code where the content of an alignment
 * widget is usually unknown, and it might make sense to, say, center a child
 * that is not stretchable, and OTOH to stretch a child that is stretchable.
 *
 * An optional background pixmap can be specified as the first argument.
 * UIs that support background pixmaps will then use the specified file
 * as a (tiled) backgound image.
 *
 * If that name does not start with "/" or ".", the theme path
 * ("/usr/share/YaST2/theme/current/") will be prepended.
 **/

YWidget *
YCPDialogParser::parseAlignment( YWidget * parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr,
				 YAlignmentType horAlign, YAlignmentType vertAlign )
{
    int		argc		= term->size() - argnr;
    YCPTerm	childTerm	= YCPNull();
    string	background_pixmap;


    if ( argc == 1 &&				// Simple case: `Center( widget )
	 term->value( argnr )->isTerm() )
    {
	childTerm = term->value( argnr )->asTerm();
    }
    else if ( argc == 2 &&		// `Center( `BackgroundPixmap( "somedir/pixmap.png" ), widget )
	      term->value( argnr   )->isTerm() &&
	      term->value( argnr   )->asTerm()->name() == YUISymbol_BackgroundPixmap &&
	      term->value( argnr   )->asTerm()->value(0)->isString() &&
	      term->value( argnr+1 )->isTerm() )
    {
	background_pixmap = term->value( argnr )->asTerm()->value(0)->asString()->value();
	childTerm = term->value( argnr+1 )->asTerm();
    }
    else
    {
	THROW_BAD_ARGS( term );
    }

    if ( YUI::app()->reverseLayout() )
    {
	if 	( horAlign == YAlignBegin )	horAlign = YAlignEnd;
	else if ( horAlign == YAlignEnd   )	horAlign = YAlignBegin;
    }

    rejectAllOptions( term, optList );
    YAlignment * alignment = YUI::widgetFactory()->createAlignment( parent, horAlign, vertAlign );

    if ( ! background_pixmap.empty() )
	alignment->setBackgroundPixmap( background_pixmap );

    parseWidgetTreeTerm( alignment, childTerm );

    return alignment;
}



/**
 * @widgets	MinWidth MinHeight MinSize
 * @id          MinSize
 * @short	Layout minimum size
 * @class	YAlignment
 * @arg		float|integer size minimum width (for MinWidth or MinSize) or minimum heigh (for MinHeight)
 * @optarg	float|integer height (only for MinSize)
 * @arg		term child The contained child widget
 * @usage	`MinWidth( 30, InputField(`id(`name), "Name" ) );
 * @example	MinWidth1.ycp MinHeight1.ycp MinSize1.ycp
 *
 * @description
 *
 * This widget makes sure its one child never gets less screen space than the specified amount.
 * It implicitly makes the child stretchable in that dimension.
 **/

YWidget *
YCPDialogParser::parseMinSize( YWidget * parent, YWidgetOpt & opt,
			       const YCPTerm & term, const YCPList & optList, int argnr,
			       bool hor, bool vert )
{
    int		argc		= term->size() - argnr;
    float	minWidth	= 0.0;
    float	minHeight	= 0.0;
    YCPTerm	childTerm	= YCPNull();

    if ( hor && vert )
    {
	if ( argc != 3 ||
	     ! isNum ( term->value( argnr   ) ) ||
	     ! isNum ( term->value( argnr+1 ) ) ||
	     ! term->value( argnr+2 )->isTerm() )
	{
	    THROW_BAD_ARGS( term );
	}

	minWidth  = toFloat( term->value( argnr   ) );
	minHeight = toFloat( term->value( argnr+1 ) );
	childTerm = term->value( argnr+2 )->asTerm();
    }
    else
    {
	if ( argc != 2 ||
	     ! isNum ( term->value( argnr   ) ) ||
	     ! term->value( argnr+1 )->isTerm() )
	{
	    THROW_BAD_ARGS( term );
	}

	if   ( hor )	minWidth  = toFloat( term->value( argnr ) );
	else		minHeight = toFloat( term->value( argnr ) );

	childTerm = term->value( argnr+1 )->asTerm();
    }


    rejectAllOptions( term, optList );

    YAlignment * alignment = YUI::widgetFactory()->createMinSize( parent, minWidth, minHeight );
    parseWidgetTreeTerm( alignment, childTerm );

    return alignment;
}



/**
 * @widget	MarginBox
 * @id          MarginBox
 * @short	Margins around one child widget
 * @class	YAlignment
 * @arg		float horMargin	 margin left and right of the child widget
 * @arg		float vertMargin margin above and below the child widget
 * @arg		term child The contained child widget
 * @usage	`MarginBox( 0.2, 0.3, `Label( "Hello" ) );
 * @usage	`MarginBox( `leftMargin( 0.7,), `rightMargin( 2.0 ), `topMargin( 0.3 ), `bottomMargin( 0.8 ), `Label( "Hello" ) );
 * @example	MarginBox1.ycp MarginBox2.ycp
 *
 * @description
 *
 * This widget is a shorthand to add margins to the sides of a child widget
 * (which may of course also be a VBox or a HBox, i.e. several widgets).
 *
 * Unlike more complex constructs like nested VBox and HBox widgets with
 * VSpacing and HSpacing at the sides, the margins of a MarginBox have lower
 * layout priorities than the real content, so if screen space becomes scarce,
 * the margins will be reduced first, and only if the margins are zero, the
 * content will be reduced in size.
 **/

YWidget *
YCPDialogParser::parseMarginBox( YWidget * parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr )
{
    float 	leftMargin	= 0.0;
    float 	rightMargin	= 0.0;
    float 	topMargin 	= 0.0;
    float	bottomMargin	= 0.0;

    bool 	paramOK		= false;
    int		argc		= term->size() - argnr;
    YCPTerm	childTerm	= YCPNull();


    if ( argc == 3 &&				// `MarginBox( horMargin, vertMargin, child )
	 isNum( term->value( argnr   ) ) &&
	 isNum( term->value( argnr+1 ) ) &&
	 term->value( argnr+2 )->isTerm() )
    {
	leftMargin = rightMargin  = toFloat( term->value( argnr   ) );
	topMargin  = bottomMargin = toFloat( term->value( argnr+1 ) );
	childTerm  = term->value( argnr+2 )->asTerm();
	paramOK    = true;
    }

    if ( ! paramOK && argc == 5 ) // `MarginBox(`leftMargin(99), `rightMargin(99), `topMargin(99), `bottomMargin(99), child );
    {
 	paramOK = term->value( argnr+4)->isTerm();

	for ( int i=argnr; i < argnr+4 && paramOK; i++ )
	{
	    if ( term->value(i)->isTerm() )
	    {
		YCPTerm marginTerm = term->value(i)->asTerm();

		if ( marginTerm->size() == 1 && isNum( marginTerm->value(0) ) )
		{
		    float margin = toFloat( marginTerm->value(0) );
		    if      ( marginTerm->name() == YUISymbol_leftMargin   )	leftMargin   = margin;
		    else if ( marginTerm->name() == YUISymbol_rightMargin  )	rightMargin  = margin;
		    else if ( marginTerm->name() == YUISymbol_topMargin    )	topMargin    = margin;
		    else if ( marginTerm->name() == YUISymbol_bottomMargin )	bottomMargin = margin;
		    else							paramOK = false;
		}
		else paramOK = false;
	    }
	    else paramOK = false;

	    if ( ! paramOK )
		ycperror( "Bad margin specification: %s", term->value(i)->toString().c_str() );
	}

	if ( paramOK )
	    childTerm  = term->value( argnr+4 )->asTerm();
    }

    if ( ! paramOK )
    {
	THROW_BAD_ARGS( term );
    }


    rejectAllOptions( term, optList );

    YAlignment * marginBox = YUI::widgetFactory()->createMarginBox( parent, leftMargin, rightMargin, topMargin, bottomMargin );
    parseWidgetTreeTerm( marginBox, childTerm );

    return marginBox;
}



/**
 * @widgets	Frame
 * @short	Frame with label
 * @class	YFrame
 * @arg		string label title to be displayed on the top left edge
 * @arg		term child the contained child widget
 * @usage	`Frame( `RadioButtonGroup( `id( rb ), `VBox( ... ) ) );
 * @examples	Frame1.ycp Frame2.ycp InputField5.ycp
 *
 * @description
 *
 * This widget draws a frame around its child and displays a title label within
 * the top left edge of that frame. It is used to visually group widgets
 * together. It is very common to use a frame like this around radio button
 * groups.
 *
 **/

YWidget *
YCPDialogParser::parseFrame( YWidget * parent, YWidgetOpt & opt,
			     const YCPTerm & term, const YCPList & optList, int argnr )
{

    int s = term->size() - argnr;
    if ( s != 2
	 || ! term->value( argnr )->isString()
	 || ! term->value( argnr+1 )->isTerm() )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term, optList );
    string label = term->value( argnr++ )->asString()->value();
    YFrame * frame = YUI::widgetFactory()->createFrame( parent, label );
    parseWidgetTreeTerm( frame, term->value( argnr )->asTerm() );

    return frame;
}



/**
 * @widgets	HSquash VSquash HVSquash
 * @id          Squash
 * @short	Layout aid: Minimize widget to its preferred size
 * @class	YSquash
 * @arg		term child the child widget
 * @usage	HSquash( `InputField( "Name:" ) )
 * @example	HSquash1.ycp
 *
 * @description
 *
 * The Squash widgets are used to control the layout. A <tt>HSquash</tt> widget
 * makes its child widget <b>nonstretchable</b> in the horizontal dimension.
 * A <tt>VSquash</tt> operates vertically, a <tt>HVSquash</tt> in both
 * dimensions.
 *
 * You can used this for example to reverse the effect of
 * <tt>`Left</tt> making a widget stretchable. If you want to make a VBox
 * containing for left aligned CheckBoxes, but want the VBox itself to be
 * non-stretchable and centered, than you enclose each CheckBox with a
 * <tt>`Left( .. )</tt> and the whole VBox with a <tt>HSquash( ... )</tt>.
 *
 *
 **/

YWidget *
YCPDialogParser::parseSquash( YWidget * parent, YWidgetOpt & opt,
			      const YCPTerm & term, const YCPList & optList, int argnr,
			      bool horSquash, bool vertSquash )
{
    if ( term->size() != argnr+1 ||
	 term->value( argnr ).isNull() ||
	 ! term->value( argnr )->isTerm() )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );
    YSquash * squash = YUI::widgetFactory()->createSquash( parent, horSquash, vertSquash );
    parseWidgetTreeTerm( squash, term->value( argnr )->asTerm() );

    return squash;
}



/**
 * @widgets	HWeight VWeight
 * @id          Weight
 * @short	Control relative size of layouts
 * @class	(YWeight)
 * @arg		integer weight the new weight of the child widget
 * @arg		term child the child widget
 * @usage	`HWeight( 2, `SelectionBox( "Language" ) )
 * @examples	Weight1.ycp
 *		Layout-Buttons-Equal-Even-Spaced1.ycp
 *		Layout-Buttons-Equal-Even-Spaced2.ycp
 *		Layout-Buttons-Equal-Growing.ycp
 *		Layout-Mixed.ycp
 *		Layout-Weights1.ycp
 *		Layout-Weights2.ycp
 *
 * @description
 *
 *
 * This widget is used to control the layout. When a <tt>HBox</tt> or
 * <tt>VBox</tt> widget decides how to devide remaining space amount two
 * <b>stretchable</b> widgets, their weights are taken into account. This
 * widget is used to change the weight of the child widget.  Each widget has a
 * vertical and a horizontal weight. You can change on or both of them.	 If you
 * use <tt>HVWeight</tt>, the weight in both dimensions is set to the same
 * value.
 *
 * Note: No real widget is created (any more), just the weight value is
 * passed to the child widget.
 *
 **/

YWidget *
YCPDialogParser::parseWeight( YWidget * parent, YWidgetOpt & opt,
			      const YCPTerm & term, const YCPList & optList, int argnr,
			      YUIDimension dim )
{
    if ( term->size() != argnr + 2
	 || !term->value(argnr)->isInteger()
	 || !term->value(argnr+1)->isTerm())
    {
	THROW_BAD_ARGS( term );
    }
    rejectAllOptions( term,optList );

    // Create child widget tree
    YWidget * child = parseWidgetTreeTerm( parent, term->value( argnr+1 )->asTerm() );

    /*
     * This is an exception from the general rule: No YWeight widget is created,
     * the weight is just set on the child widget.
     * The YWeight widget is plain superfluos - YWidget can handle everything itself.
     */

    int weight = term->value( argnr )->asInteger()->value();
    child->setWeight( dim, weight );

    return child;
}



/**
 * @widgets	HBox VBox
 * @id          Box
 * @short	Generic layout: Arrange widgets horizontally or vertically
 * @class	LayoutBox
 * @optarg	term child1 the first child widget
 * @optarg	term child2 the second child widget
 * @optarg	term child3 the third child widget
 * @optarg	term child4 the fourth child widget ( and so on... )
 * @option	debugLayout verbose logging
 * @usage	HBox( `PushButton( `id( `ok ), "OK" ), `PushButton( `id( `cancel ), "Cancel" ) )
 *
 * @examples	VBox1.ycp HBox1.ycp
 *		Layout-Buttons-Equal-Growing.ycp
 *		Layout-Fixed.ycp
 *		Layout-Mixed.ycp
 *
 * @description
 *
 * The layout boxes are used to split up the dialog and layout a number of
 * widgets horizontally ( <tt>HBox</tt> ) or vertically ( <tt>VBox</tt> ).
 *
 **/

YWidget *
YCPDialogParser::parseLayoutBox( YWidget * parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr,
				 YUIDimension dim )
{
    // Parse options

    bool debugLayout = false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if   ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_debugLayout ) debugLayout = true;
	else logUnknownOption( term, optList->value(o) );
    }

    YLayoutBox * layoutBox = YUI::widgetFactory()->createLayoutBox( parent, dim );

    if ( debugLayout )
	layoutBox->setDebugLayout();

    for ( int w=argnr; w < term->size(); w++ )
    {
	parseWidgetTreeTerm( layoutBox, term->value(w)->asTerm() );
    }

    return layoutBox;
}



/**
 * @widgets	ButtonBox
 * @id          ButtonBox
 * @short	Layout for push buttons that takes button order into account
 * @class	ButtonBox
 * @arg		term button1 the first button
 * @optarg	term button2 the second button (etc.)
 * @option	relaxSanityCheck less stringent requirements for button roles
 * @usage	`ButtonBox(`PushButton( `id( `ok ), "OK" ), `PushButton( `id( `cancel ), "Cancel" ) )
 *
 * @examples	ButtonBox1.ycp
 *
 * @description
 *
 * This widget arranges its push button child widgets according to the current
 * button order.
 *
 * The button order depends on what UI is used and (optionally) what desktop
 * environment the UI currently runs in.
 *
 * The Qt and NCurses UIs use the KDE / Windows button order:
 *
 *     [OK] [Apply] [Cancel] [Custom1] [Custom2] ... [Help]
 *
 *     [Continue] [Cancel]
 *
 *     [Yes] [No]
 *
 *
 * The Gtk UI uses the GNOME / MacOS button order:
 *
 *     [Help] [Custom1] [Custom2] ... [Apply] [Cancel] [OK]
 *
 *     [Cancel] [Continue]
 *
 *     [No] [Yes]
 *
 *
 * Certain buttons have a predefined role:
 *
 * - okButton: Positive confirmation: Use the values from the dialog to do
 *   whatever the dialog is all about and close the dialog.
 *
 * - applyButton: Use the values from the dialog, but leave the dialog open.
 *
 * - cancelButton: Discard all changes and close the dialog.
 *
 * - helpButton: Show help for this dialog.
 *
 * In a [Continue] [Cancel] dialog, [Continue] has the okButton role.
 * In a [Yes] [No] dialog, [Yes] has the okButton role, [No] has the
 * cancelButton role.
 *
 * The UI automatically recognizes standard button labels and assigns the
 * proper role. This is done very much like assigning function keys (see
 * UI::SetFunctionKeys()). The UI also has some built-in heuristics to
 * recognize standard button IDs like `id(`ok), `id("ok"), `id(`yes), etc.
 *
 * Sometimes it makes sense to use something like [Print] or [Delete] for the
 * okButton role if printing or deleting is what the respective dialog is all
 * about. In that case, the application has to explicitly specify that button
 * role: Use `opt(`okButton).
 *
 * Similarly, there are `opt(`cancelButton), `opt(`applyButton),
 * `opt(`helpButton).
 *
 * By default, a ButtonBox with more than one button is required to have one
 * okButton and one cancelButton.
 * `opt(`relaxSanityCheck) relaxes those requirements: It does not check for
 * one okButton and one cancelButton. This should be used very sparingly -- use
 * your common sense. One Example where this is legitimate is a pop-up dialog
 * with [OK] [Details] for error messages that can be explained in more
 * detail. Most dialogs with more than just an [OK] or a [Close] button should
 * have a [Cancel] button.
 *
 * ButtonBox widgets can have no other child widgets than PushButton widgets.
 * ButtonBox widgets are horizontally stretchable and vertically
 * non-stretchable. If there is more space, their layout policy (depending on
 * KDE or GNOME button order) specifies whether to center or right-align the
 * buttons. 
 **/

YWidget *
YCPDialogParser::parseButtonBox( YWidget * parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr )
{
    // Parse options

    bool relaxSanityCheck = false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if   ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_relaxSanityCheck ) relaxSanityCheck = true;
	else logUnknownOption( term, optList->value(o) );
    }

    YButtonBox * buttonBox = YUI::widgetFactory()->createButtonBox( parent );
    YUI_CHECK_NEW( buttonBox );

    for ( int buttonNo=argnr; buttonNo < term->size(); buttonNo++ )
    {
	YWidgetOpt opt;
	YWidget     * child  = parseWidgetTreeTerm( buttonBox, opt, term->value( buttonNo )->asTerm() );
	YPushButton * button = dynamic_cast<YPushButton *> (child);
	YUI_CHECK_PTR( button );

	if ( button->role() == YCustomButton && ! opt.customButton.value() && button->hasId() )
	{
	    // Try to guess something better from the widget ID

	    string id = button->id()->toString();

	    if ( id.size() > 0 && id[0] == '`' ) // get rid of backtick, if there is one
		id = id.erase( 0, 1 ); // erase 1 character starting from pos. 0

	    // STL strings don't have anything like strncasecmp()

	    YButtonRole role = YCustomButton;

	    if 		( startsWith( id, "ok"  	) )	role = YOKButton;
	    else if 	( startsWith( id, "yes" 	) )	role = YOKButton;
	    else if 	( startsWith( id, "continue" 	) )	role = YOKButton;
	    else if 	( startsWith( id, "accept" 	) )	role = YOKButton;
	    
	    else if 	( startsWith( id, "cancel" 	) )	role = YCancelButton;
	    else if 	( startsWith( id, "no" 		) )	role = YCancelButton;
	    else if 	( startsWith( id, "apply" 	) )	role = YApplyButton;
	    else if 	( startsWith( id, "help" 	) )	role = YHelpButton;

	    if ( role != YCustomButton )
	    {
		button->setRole( role );
		yuiMilestone() << "Guessed button role " << role
			       << " for " << button << " from widget ID"
			       << endl;
	    }
	    else
	    {
		yuiDebug() << "No guess for a button role for ID " << id
			   << " of " << button
			   << endl;
	    }
	}
    }

    try
    {
	if ( relaxSanityCheck )
	{
	    yuiMilestone() << "Relaxed sanity check for " << buttonBox << endl;
	    buttonBox->setSanityCheckRelaxed( relaxSanityCheck );
	}

	buttonBox->sanityCheck();
    }
    catch ( YUIException & exception )
    {
	YUI_CAUGHT( exception);
	ycperror( "Bad ButtonBox content" );
	YUI_RETHROW( exception );
    }

    return buttonBox;
}


bool YCPDialogParser::startsWith( const string & str, const char * word )
{
    return strncasecmp( str.c_str(), word, strlen( word ) )== 0;
}



/**
 * @widgets	Label Heading
 * @short	Simple static text
 * @class	YLabel
 * @arg		string label
 * @option	outputField make the label look like an input field in read-only mode
 * @option	boldFont use a bold font
 * @usage	`Label( "Here goes some text\nsecond line" )
 *
 * @examples	Label1.ycp Label2.ycp Label3.ycp Label4.ycp
 *		Heading1.ycp Heading2.ycp Heading3.ycp
 *
 * @description
 *
 * A <tt>Label</tt> is static text displayed in the dialog.
 * A <tt>Heading</tt> is static text with a bold and/or larger font.
 * In both cases, the text may contain newlines.
 **/

YWidget *
YCPDialogParser::parseLabel( YWidget * parent, YWidgetOpt & opt,
			     const YCPTerm & term, const YCPList & optList, int argnr,
			     bool isHeading )
{
    if ( term->size() - argnr != 1
	 || !term->value(argnr)->isString())
    {
	THROW_BAD_ARGS( term );
    }


    // Parse options

    bool isOutputField = false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_outputField ) isOutputField = true;
	else logUnknownOption( term, optList->value(o) );
    }

    string labelText = term->value( argnr )->asString()->value();

    if ( isHeading )
	isOutputField = false;

    YLabel * label = YUI::widgetFactory()->createLabel( parent, labelText, isHeading, isOutputField );

    if ( opt.boldFont.value() )
	label->setUseBoldFont();

    return label;
}



/**
 * @widget	RichText
 * @short	Static text with HTML-like formatting
 * @class	YRichText
 * @arg		string text
 * @option	plainText don't interpret text as HTML
 * @option	autoScrollDown automatically scroll down for each text change
 * @option	shrinkable make the widget very small
 * @usage	`RichText( "This is a <b>bold</b> text" )
 * @example	RichText1.ycp RichText2.ycp RichText3.ycp RichText-hyperlinks.ycp
 *
 * @description
 *
 *
 * A <tt>RichText</tt> is a text area with two major differences to a
 * <tt>Label</tt>: The amount of data it can contain is not restricted by the
 * layout and a number of control sequences are allowed, which control the
 * layout of the text.
 *
 **/

YWidget *
YCPDialogParser::parseRichText( YWidget * parent, YWidgetOpt & opt,
				const YCPTerm & term, const YCPList & optList, int argnr )
{
    if ( term->size() - argnr != 1
	 || !term->value(argnr)->isString())
    {
	THROW_BAD_ARGS( term );
    }

    string 	text 		= term->value( argnr )->asString()->value();
    bool 	plainTextMode	= false;
    bool 	autoScrollDown	= false;
    bool 	shrinkable	= false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if ( optList->value(o)->isSymbol() )
	{
	    string sym = optList->value(o)->asSymbol()->symbol();

	    if	    ( sym  == YUIOpt_plainText	    ) 	plainTextMode  = true;
	    else if ( sym  == YUIOpt_autoScrollDown ) 	autoScrollDown = true;
	    else if ( sym  == YUIOpt_shrinkable	    )	shrinkable     = true;
	    else    logUnknownOption( term, optList->value(o) );
	}
	else logUnknownOption( term, optList->value(o) );
    }

    YRichText * richText = YUI::widgetFactory()->createRichText( parent, text, plainTextMode );

    if ( autoScrollDown )	richText->setAutoScrollDown( true );
    if ( shrinkable	)	richText->setShrinkable( true );

    return richText;
}



/**
 * @widget	LogView
 * @short	scrollable log lines like "tail -f"
 * @class	YLogView
 * @arg		string label (above the log lines)
 * @arg		integer visibleLines number of visible lines (without scrolling)
 * @arg		integer maxLines number of log lines to store (use 0 for "all")
 * @usage	`LogView( "Log file", 4, 200 );
 * @example	LogView1.ycp
 *
 * @description
 *
 *
 * A scrolled output-only text window where ASCII output of any kind can be
 * redirected - very much like a shell window with "tail -f".
 *
 * The LogView will keep up to "maxLines" of output, discarding the oldest
 * lines if there are more. If "maxLines" is set to 0, all lines will be kept.
 *
 * "visibleLines" lines will be visible by default (without scrolling) unless
 * you stretch the widget in the layout.
 *
 * Use <tt>ChangeWidget( `id( `log ), `LastLine, "bla blurb...\n" )</tt> to append
 * one or several line(s) to the output. Notice the newline at the end of each line!
 *
 * Use <tt>ChangeWidget( `id( `log ), `Value, "bla blurb...\n" )</tt> to replace
 * the entire contents of the LogView.
 *
 * Use <tt>ChangeWidget( `id( `log ), `Value, "" )</tt> to clear the contents.
 **/

YWidget *
YCPDialogParser::parseLogView( YWidget * parent, YWidgetOpt & opt,
			       const YCPTerm & term, const YCPList & optList, int argnr )
{
    if ( term->size() - argnr != 3
	 || ! term->value( argnr   )->isString()
	 || ! term->value( argnr+1 )->isInteger()
	 || ! term->value( argnr+2 )->isInteger())
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    string	label		= term->value( argnr   )->asString()->value();
    int 	visibleLines	= term->value( argnr+1 )->asInteger()->value();
    int 	maxLines	= term->value( argnr+2 )->asInteger()->value();

    return YUI::widgetFactory()->createLogView( parent, label, visibleLines, maxLines );
}



/**
 * @widget	PushButton IconButton
 * @short	Perform action on click
 * @class	YPushButton
 * @arg		string iconName (IconButton only)
 * @arg		string label
 * @option	default makes this button the dialogs default button
 * @option	helpButton automatically shows topmost `HelpText
 * @option	okButton     assign the [OK] role to this button (see ButtonBox)
 * @option	cancelButton assign the [Cancel] role to this button (see ButtonBox)
 * @option	applyButton  assign the [Apply] role to this button (see ButtonBox)
 * @option	customButton override any other button role assigned to this button
 * @usage	`PushButton( `id( `click ), `opt( `default, `hstretch ), "Click me" )
 * @examples	PushButton1.ycp PushButton2.ycp IconButton1.ycp
 *
 * @description
 *
 *
 * A <tt>PushButton</tt> is a button with a text label the user can
 * press in order to activate some action. If you call <tt>UserInput()</tt> and
 * the user presses the button, <tt>UserInput()</tt> returns with the id of the
 * pressed button.
 *
 * You can (and should) provide keybord shortcuts along with the button
 * label. For example "&amp; Apply" as a button label will allow the user to
 * activate the button with Alt-A, even if it currently doesn't have keyboard
 * focus. This is important for UIs that don't support using a mouse.
 *
 * An <tt>IconButton</tt> is pretty much the same, but it has an icon in
 * addition to the text. If the UI cannot handle icons, it displays only the
 * text, and the icon is silently omitted.
 *
 * Icons are (at the time of this writing) loaded from the <em>theme</em>
 * directory, /usr/share/YaST2/theme/current.
 *
 * If a button has `opt(`helpButton) set, it is the official help button of
 * this dialog. When activated, this will open a new dialog with the topmost
 * help text in this dialog (the topmost widget that has a property `HelpText)
 * in a pop-up dialog with a local event loop. Note that this is not done
 * during UI::PollInput() to prevent the application from blocking as long as
 * the help dialog is open.
 *
 * Since a help button is handled internally by the UI, UI::UserInput() and
 * related will never return this button's ID.
 **/

YWidget *
YCPDialogParser::parsePushButton( YWidget * parent, YWidgetOpt & opt,
				  const YCPTerm & term, const YCPList & optList, int argnr,
				  bool isIconButton )
{
    string 	label;
    string 	iconName;
    bool   	isDefaultButton = false;
    YButtonRole	role            = YCustomButton;

    if ( isIconButton )
    {
	if ( term->size() - argnr != 2
	     || ! term->value(argnr)->isString()
	     || ! term->value(argnr+1)->isString() )
	{
	    THROW_BAD_ARGS( term );
	}

	iconName = term->value( argnr	)->asString()->value();
	label	 = term->value( argnr+1 )->asString()->value();
    }
    else
    {
	if ( term->size() - argnr != 1
	     || !term->value(argnr)->isString() )
	{
	    THROW_BAD_ARGS( term );
	}

	label = term->value( argnr )->asString()->value();
    }

    // Parse options

    for ( int o=0; o < optList->size(); o++ )
    {
	if ( optList->value(o)->isSymbol() )
	{
	    string sym = optList->value(o)->asSymbol()->symbol();

	    if	    ( sym == YUIOpt_default    	)	isDefaultButton = true;
	    else if ( sym == YUIOpt_okButton	)	role = YOKButton;
	    else if ( sym == YUIOpt_cancelButton)	role = YCancelButton;
	    else if ( sym == YUIOpt_applyButton)	role = YApplyButton;
	    else if ( sym == YUIOpt_helpButton 	)	role = YHelpButton;
	    else if ( sym == YUIOpt_customButton)	opt.customButton.setValue( true );
	    else logUnknownOption( term, optList->value(o) );
	}
	else logUnknownOption( term, optList->value(o) );
    }

    YPushButton * button = YUI::widgetFactory()->createPushButton( parent, label );

    if ( role != YCustomButton ) // The button constructor might have guessed something else
	button->setRole( role );

    if ( isDefaultButton )
	button->setDefaultButton();

    if ( role == YHelpButton )
	button->setHelpButton();

    if ( isIconButton )
	button->setIcon( iconName );

    return button;
}


/**
 * @widget	MenuButton
 * @short	Button with popup menu
 * @class	YMenuButton
 * @arg		string		label
 * @arg		itemList	menu items
 * @usage	`MenuButton( "button label", [ `item( `id( `doit ), "&amp; Do it" ), `item( `id( `something ), "&amp; Something" ) ] );
 * @examples	MenuButton1.ycp MenuButton2.ycp
 *
 * @description
 *
 * This is a widget that looks very much like a <tt>PushButton</tt>, but unlike
 * a <tt>PushButton</tt> it doesn't immediately start some action but opens a
 * popup menu from where the user can select an item that starts an action. Any
 * item may in turn open a submenu etc.
 *
 * <tt>UserInput()</tt> returns the ID of a menu item if one was activated. It
 * will never return the ID of the <tt>MenuButton</tt> itself.
 *
 * <b>Style guide hint:</b> Don't overuse this widget. Use it for dialogs that
 * provide lots of actions. Make the most frequently used actions accessible
 * via normal <tt>PushButtons</tt>. Move less frequently used actions
 * (e.g. "expert" actions) into one or more <tt>MenuButtons</tt>. Don't nest
 * the popup menus too deep - the deeper the nesting, the more awkward the user
 * interface will be.
 *
 * You can (and should) provide keybord shortcuts along with the button
 * label as well as for any menu item.
 *
 **/

YWidget *
YCPDialogParser::parseMenuButton( YWidget * parent, YWidgetOpt & opt,
				  const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numargs = term->size() - argnr;

    if ( numargs < 1 || numargs > 2
	 || ! term->value( argnr )->isString()
	 || ( numargs >= 2 && ! term->value( argnr+1 )->isList() ) )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    string label   = term->value( argnr )->asString()->value();

    YMenuButton * menuButton = YUI::widgetFactory()->createMenuButton( parent, label );

    if ( numargs >= 2 )
    {
	YCPList itemList = term->value( argnr+1 )->asList();
	menuButton->addItems( YCPMenuItemParser::parseMenuItemList( itemList ) );
    }

    return menuButton;
}


/**
 * @widget	CheckBox
 * @short	Clickable on/off toggle button
 * @class	YCheckBox
 * @arg		string label the text describing the check box
 * @optarg	boolean|nil checked whether the check box should start checked -
 *		nil means tristate condition, i.e. neither on nor off
 * @option	boldFont use a bold font
 * @usage	`CheckBox( `id( `cheese ), "&amp; Extra cheese" )
 * @examples	CheckBox1.ycp CheckBox2.ycp CheckBox3.ycp CheckBox4.ycp
 *
 * @description
 *
 * A checkbox widget has two states: Checked and not checked. It returns no
 * user input but you can query and change its state via the <tt>Value</tt>
 * property.
 **/

YWidget *
YCPDialogParser::parseCheckBox( YWidget * parent, YWidgetOpt & opt,
				const YCPTerm & term, const YCPList & optList, int argnr )
{
    int size = term->size() - argnr;
    if ( size < 1 || size > 2
	 || !term->value( argnr )->isString()
	 || ( size == 2 && ! term->value(argnr+1)->isBoolean() ) )
    {
	THROW_BAD_ARGS( term );
    }
    rejectAllOptions( term,optList );

    string label   = term->value( argnr )->asString()->value();
    bool   checked = false;

    if ( size == 2 )
	checked = term->value( argnr+1 )->asBoolean()->value();

    YCheckBox * checkBox = YUI::widgetFactory()->createCheckBox( parent, label, checked );

    if ( opt.boldFont.value() )
	checkBox->setUseBoldFont();

    return checkBox;
}


/**
 * @widget	CheckBoxFrame
 * @short	Frame with clickable on/off toggle button
 * @class	YCheckBoxFrame
 * @arg		string label the text describing the check box
 * @arg		boolean checked whether the check box should start checked
 * @arg		term child the child widgets for frame content - typically
 *		`VBox(...) or `HBox(...)
 * @option	noAutoEnable do not enable/disable frame children upon status change
 * @option	invertAutoAnable disable frame children if check box is checked
 * @usage	`CheckBoxFrame( `id( `custom), "&Custom", true, `VBox(`InputField(...), ... )
 * @examples	CheckBoxFrame1.ycp CheckBoxFrame2.ycp CheckBoxFrame3.ycp
 *
 * @description
 *
 * This is a combination of the check box widget and the frame widget:
 * A frame that has a check box where a simple frame would have its frame title.
 *
 * By default, the frame content (the child widgets) get disabled if the check box
 * is set to "off" (unchecked) and enabled if the check box is set to "on" (cheched).
 *
 * `opt(`invertAutoEnable) inverts this behaviour: It makes YCheckBoxFrame
 * disable its content (its child widgets) if it is set to "on" (checked) and
 * enable its content if it is set to "off".
 *
 * `opt(`noAutoEnable) switches off disabling and enabling the frame content (the
 * child widgets) completely. In that case, use QueryWidget() and/or
 * `opt(`immediate).
 *
 * Please note that unlike YCheckBox this widget does not support tri-state -
 * it is always either on or off.
 **/

YWidget *
YCPDialogParser::parseCheckBoxFrame( YWidget * parent, YWidgetOpt & opt,
				     const YCPTerm & term, const YCPList & optList, int argnr )
{
    int s = term->size() - argnr;
    if ( s != 3
	 || ! term->value( argnr   )->isString()
	 || ! term->value( argnr+1 )->isBoolean()
	 || ! term->value( argnr+2 )->isTerm()
	 )
    {
	THROW_BAD_ARGS( term );
    }

    string	label		 = term->value( argnr   )->asString()->value();
    bool	checked 	 = term->value( argnr+1 )->asBoolean()->value();
    YCPTerm	childTerm	 = term->value( argnr+2 )->asTerm();
    bool	autoEnable	 = true;
    bool	invertAutoEnable = false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if ( optList->value(o)->isSymbol() )
	{
	    string sym = optList->value(o)->asSymbol()->symbol();

	    if	    ( sym  == YUIOpt_invertAutoEnable	) invertAutoEnable = true;
	    else if ( sym  == YUIOpt_noAutoEnable	) autoEnable	   = false;
	    else logUnknownOption( term, optList->value(o) );
	}
	else logUnknownOption( term, optList->value(o) );
    }

    if ( invertAutoEnable && ! autoEnable )
    {
	yuiWarning() << ( "`opt(noAutoEnable) automatically disables `opt(`invertAutoEnable)" );
	invertAutoEnable = false;
    }

    YCheckBoxFrame * checkBoxFrame = YUI::widgetFactory()->createCheckBoxFrame( parent, label, checked );

    if ( ! autoEnable )		checkBoxFrame->setAutoEnable( false );
    if ( invertAutoEnable )	checkBoxFrame->setInvertAutoEnable( true );

    parseWidgetTreeTerm( checkBoxFrame, childTerm );
    checkBoxFrame->handleChildrenEnablement( checked );

    return checkBoxFrame;
}



/**
 * @widget	RadioButton
 * @short	Clickable on/off toggle button for radio boxes
 * @class	YRadioButton
 * @arg		string label
 * @optarg	boolean selected
 * @option	boldFont use a bold font
 * @usage	`RadioButton( `id( `now ), "Crash now", true )
 * @examples	RadioButton1.ycp RadioButton2.ycp Frame2.ycp ShortcutConflict3.ycp
 *
 * @description
 *
 * A radio button is not usefull alone. Radio buttons are group such that the
 * user can select one radio button of a group. It is much like a selection
 * box, but radio buttons can be dispersed over the dialog.  Radio buttons must
 * be contained in a <tt>RadioButtonGroup</tt>.
 **/

YWidget *
YCPDialogParser::parseRadioButton( YWidget * parent, YWidgetOpt & opt,
				   const YCPTerm & term, const YCPList & optList, int argnr )
{

    int s = term->size() - argnr;
    if ( s < 1 || s > 2
	 || ! term->value( argnr )->isString()
	 || ( s == 2 && ! term->value( argnr+1 )->isBoolean() ) )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    string label     = term->value( argnr )->asString()->value();
    bool   isChecked = false;
    if ( s == 2 )
	isChecked = term->value( argnr+1 )->asBoolean()->value();

    YRadioButton * radioButton = YUI::widgetFactory()->createRadioButton( parent, label, isChecked );

    if ( opt.boldFont.value() )
	radioButton->setUseBoldFont();

    return radioButton;
}



/**
 * @widget	RadioButtonGroup
 * @short	Radio box - select one of many radio buttons
 * @class	YRadioButtonGroup
 * @arg		term child the child widget
 * @usage	`RadioButtonGroup( `id( rb ), `VBox( ... ) )
 * @examples	RadioButton1.ycp Frame2.ycp
 *
 * @description
 *
 * A <tt>RadioButtonGroup</tt> is a container widget that has neither impact on
 * the layout nor has it a graphical representation. It is just used to
 * logically group RadioButtons together so the one-out-of-many selection
 * strategy can be ensured.
 *
 * Radio button groups may be nested.  Looking bottom up we can say that a
 * radio button belongs to the radio button group that is nearest to it. If you
 * give the <tt>RadioButtonGroup</tt> widget an id, you can use it to query and
 * set which radio button is currently selected.
 *
 **/

YWidget *
YCPDialogParser::parseRadioButtonGroup( YWidget * parent, YWidgetOpt & opt,
					const YCPTerm & term, const YCPList & optList, int argnr )
{
    if ( term->size() != argnr+1
	 || !term->value(argnr)->isTerm())
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    YRadioButtonGroup * radioButtonGroup = YUI::widgetFactory()->createRadioButtonGroup( parent );
    parseWidgetTreeTerm( radioButtonGroup, term->value( argnr )->asTerm() );

    return radioButtonGroup;
}



/**
 * @widgets	InputField TextEntry Password
 * @short	Input field
 * @class	YInputField
 * @arg		string label the label describing the meaning of the entry
 * @optarg	string defaulttext The text contained in the text entry
 * @option	shrinkable make the input field very small
 * @usage	`InputField( `id( `name ), "Enter your name:", "Kilroy" )
 *
 * @examples	InputField1.ycp InputField2.ycp InputField3.ycp InputField4.ycp
 *		InputField5.ycp InputField6.ycp
 *		Password1.ycp Password2.ycp
 *              InputField-setInputMaxLength.ycp
 *
 * @description
 *
 * This widget is a one line text entry field with a label above it. An initial
 * text can be provided.
 *
 * @note        You can and should set a keyboard shortcut within the
 * label. When the user presses the hotkey, the corresponding text entry widget
 * will get the keyboard focus.
 *
 * @note	Bug compatibility mode: If used as TextEntry(), `opt(`hstretch)
 *		is automatically added (with a warning in the log about that fact)
 *		to avoid destroying dialogs written before fixing a geometry bug
 *		in the widget. The bug had caused all TextEntry widgets to be
 *		horizontally stretchable, so they consumed all the horizontal
 *		space they could get, typically making them as wide as the dialog.
 *		When used with the new name InputField(), they use a reasonable
 *		default width. You can still add `opt(`hstretch), of course.
 **/

YWidget *
YCPDialogParser::parseInputField( YWidget * parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr,
				 bool passwordMode, bool bugCompatibilityMode )
{
    static bool postedBugCompatibilityWarning = false;

    if ( term->size() - argnr < 1 || term->size() - argnr > 2
	 || !term->value(argnr)->isString()
	 || (term->size() == argnr+2 && !term->value(argnr+1)->isString()))
    {
	THROW_BAD_ARGS( term );
    }

    string label = term->value( argnr )->asString()->value();

    string initialValue;
    if ( term->size() >= argnr + 2 )
	initialValue = term->value( argnr+1 )->asString()->value();

    bool shrinkable = false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_shrinkable ) shrinkable = true;
	else logUnknownOption( term, optList->value(o) );
    }

    YInputField * inputField = YUI::widgetFactory()->createInputField( parent, label, passwordMode );

    if ( ! initialValue.empty() )
	inputField->setValue( initialValue );

    if ( shrinkable )
	inputField->setShrinkable();
    else if ( bugCompatibilityMode )
    {
	inputField->setStretchable( YD_HORIZ, true );

	if ( ! postedBugCompatibilityWarning )
	{
	    yuiWarning() <<
		"\n"
		"\nUsing `TextEntry() in bug compatibiltiy mode."
		"\nThis is equivalent to `InputField(`opt(`hstretch), ...)."
		"\nThis makes the field grab as much space horizontally as it can get,"
		"\ntypically making it stretch across the entire width of the dialog."
		"\nWithout this hstretch, the field will take a reasonable default width."
		"\n"
		"\nIf this `hstretch is really desired, please use `InputField(`opt(`hstretch), ...)."
		"\nIf it is not, please use `InputField() without `hstretch."
		"\n\n" << endl;

	    postedBugCompatibilityWarning = true;
	}
    }


    return inputField;
}


/**
 * @widgets	MultiLineEdit
 * @short	multiple line text edit field
 * @class	YMultiLineEdit
 * @arg		string label label above the field
 * @optarg	string initialValue the initial contents of the field
 * @usage	`MultiLineEdit( `id( `descr ), "Enter problem &amp; description:", "No problem here." )
 *
 * @examples	MultiLineEdit1.ycp MultiLineEdit2.ycp MultiLineEdit3.ycp MultiLineEdit-setInputMaxLength.ycp
 *
 * @description
 *
 *
 * This widget is a multiple line text entry field with a label above it.
 * An initial text can be provided.
 *
 * Note: You can and should set a keyboard shortcut within the label. When the
 * user presses the hotkey, the corresponding MultiLineEdit widget will get the
 * keyboard focus.
 **/

YWidget *
YCPDialogParser::parseMultiLineEdit( YWidget * parent, YWidgetOpt & opt,
				     const YCPTerm & term, const YCPList & optList, int argnr )
{

    if ( term->size() - argnr < 1 || term->size() - argnr > 2
	 || !term->value(argnr)->isString()
	 || (term->size() == argnr+2 && !term->value(argnr+1)->isString()))
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term, optList );

    string label = term->value( argnr )->asString()->value();
    string initialValue;

    if ( term->size() >= argnr + 2 )
	initialValue = term->value( argnr+1 )->asString()->value();

    YMultiLineEdit * multiLineEdit = YUI::widgetFactory()->createMultiLineEdit( parent, label );

    if ( ! initialValue.empty() )
	multiLineEdit->setValue( initialValue );

    return multiLineEdit;
}



/**
 * @widget	SelectionBox
 * @short	Scrollable list selection
 * @class	YSelectionBox
 * @arg		string label
 * @optarg	list items the items contained in the selection box
 * @option	shrinkable make the widget very small
 * @option	immediate	make `notify trigger immediately when the selected item changes
 * @usage	`SelectionBox( `id( `pizza ), "select your Pizza:", [ "Margarita", `item( `id( `na ), "Napoli" ) ] )
 * @examples	SelectionBox1.ycp
 *		SelectionBox2.ycp
 *		SelectionBox3.ycp
 *		SelectionBox4.ycp
 *		SelectionBox-icons.ycp
 *		SelectionBox-replace-items1.ycp
 *		SelectionBox-replace-items2.ycp
 *
 * @description
 *
 * A selection box offers the user to select an item out of a list. Each item
 * has a label and an optional id. When constructing the list of items, you
 * have two way of specifying an item. Either you give a plain string, in which
 * case the string is used both for the id and the label of the item. Or you
 * specify a term <tt>`item( term id, string label )</tt> or <tt>`item( term id,
 * string label, boolean selected )</tt>, where you give an id of the form
 * <tt>`id( any v )</tt> where you can store an aribtrary value as id. The third
 * argument controls whether the item is the selected item.
 *
 **/

YWidget *
YCPDialogParser::parseSelectionBox( YWidget * parent, YWidgetOpt & opt,
				    const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numargs = term->size() - argnr;

    if ( numargs < 1 || numargs > 2
	 || ! term->value( argnr )->isString()
	 || ( numargs >= 2 && ! term->value( argnr+1 )->isList() ) )
    {
	THROW_BAD_ARGS( term );
    }

    string label = term->value( argnr )->asString()->value();

    bool shrinkable = false;
    bool immediate  = false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if	( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_shrinkable )	shrinkable = true;
	else if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_immediate	)	immediate  = true;
	else logUnknownOption( term, optList->value(o) );
    }

    YSelectionBox *selBox = YUI::widgetFactory()->createSelectionBox( parent, label );

    if ( shrinkable )
	selBox->setShrinkable( true );

    if ( immediate )
	selBox->setImmediateMode( true ); // includes setNotify()

    if ( numargs >= 2 )
    {
	YCPList itemList = term->value( argnr+1 )->asList();
	selBox->addItems( YCPItemParser::parseItemList( itemList ) );
    }

    return selBox;
}


/**
 * @widget	MultiSelectionBox
 * @short	Selection box that allows selecton of multiple items
 * @class	YMultiSelectionBox
 * @arg		string	label
 * @optarg	list	items	the items initially contained in the selection box
 * @option	shrinkable make the widget very small
 * @usage	`MultiSelectionBox( `id( `topping ), "select pizza toppings:", [ "Salami", `item( `id( `cheese ), "Cheese", true ) ] )
 * @examples	MultiSelectionBox1.ycp
 *		MultiSelectionBox2.ycp
 *		MultiSelectionBox3.ycp
 *		MultiSelectionBox-replace-items1.ycp
 *		MultiSelectionBox-replace-items2.ycp
 *
 * @description
 *
 * The MultiSelectionBox displays a ( scrollable ) list of items from which any
 * number (even nothing!) can be selected. Use the MultiSelectionBox's
 * <tt>SelectedItems</tt> property to find out which.
 *
 * Each item can be specified either as a simple string or as
 * <tt>`item( ... )</tt> which includes an ( optional ) ID and an (optional)
 * 'selected' flag that specifies the initial selected state ('not selected',
 * i.e. 'false', is default).
 *
 **/

YWidget *
YCPDialogParser::parseMultiSelectionBox( YWidget * parent, YWidgetOpt & opt,
					 const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numargs = term->size() - argnr;

    if ( numargs < 1 || numargs > 2
	 || ! term->value( argnr   )->isString()
	 || ( numargs >= 2 && ! term->value( argnr+1 )->isList() ) )
    {
	THROW_BAD_ARGS( term );
    }

    string label      = term->value( argnr )->asString()->value();
    bool   shrinkable = false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_shrinkable ) shrinkable = true;
	else logUnknownOption( term, optList->value(o) );
    }

    YMultiSelectionBox * multiSelectionBox = YUI::widgetFactory()->createMultiSelectionBox( parent, label );

    if ( shrinkable )
	multiSelectionBox->setShrinkable( true );

    if ( numargs >= 2 )
    {
	YCPList itemList = term->value( argnr+1 )->asList();
	multiSelectionBox->addItems( YCPItemParser::parseItemList( itemList ) );
    }

    return multiSelectionBox;
}




/**
 * @widget	ComboBox
 * @short	drop-down list selection (optionally editable)
 * @class	YComboBox
 * @arg		string label
 * @optarg	list items the items contained in the combo box
 * @option	editable the user can enter any value.
 * @usage	`ComboBox( `id( `pizza ), "select your Pizza:", [ "Margarita", `item( `id( `na ), "Napoli" ) ] )
 * @examples	ComboBox1.ycp
 *		ComboBox2.ycp
 *		ComboBox3.ycp
 *		ComboBox4.ycp
 * 		ComboBox-replace-items1.ycp
 * 		ComboBox-setInputMaxLength.ycp
 *
 * @description
 *
 * A combo box is a combination of a selection box and an input field. It gives
 * the user a one-out-of-many choice from a list of items.  Each item has a
 * ( mandatory ) label and an ( optional ) id.	When the 'editable' option is set,
 * the user can also freely enter any value. By default, the user can only
 * select an item already present in the list.
 *
 * The items are very much like SelectionBox items: They can have an (optional)
 * ID, they have a mandatory text to be displayed and an optional boolean
 * parameter indicating the selected state. Only one of the items may have this
 * parameter set to "true"; this will be the default selection on startup.
 *
 * @note         You can and should set a keyboard shortcut within the
 * label. When the user presses the hotkey, the combo box will get the keyboard
 * focus.
 *
 **/

YWidget *
YCPDialogParser::parseComboBox( YWidget * parent, YWidgetOpt & opt,
				const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numargs = term->size() - argnr;
    if ( numargs < 1 || numargs > 2
	 || !term->value(argnr)->isString()
	 || ( numargs >= 2 && ! term->value( argnr+1 )->isList() ) )
    {
	THROW_BAD_ARGS( term );
    }

    string label    = term->value( argnr )->asString()->value();
    bool   editable = false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_editable ) editable = true;
	else logUnknownOption( term, optList->value(o) );
    }

    YComboBox * comboBox = YUI::widgetFactory()->createComboBox( parent, label, editable );

    if ( numargs >= 2 )
    {
	YCPList itemList = term->value( argnr+1 )->asList();
	comboBox->addItems( YCPItemParser::parseItemList( itemList ) );
    }

    return comboBox;
}



/**
 * @widget	Tree
 * @short	Scrollable tree selection
 * @class	YTree
 * @arg		string		label
 * @optarg	itemList	items	the items contained in the tree
 *              <code>
 *		itemList ::=
 *			[
 *				item
 *				[ , item ]
 *				[ , item ]
 *				...
 *			]
 *		item ::=
 *			string |
 *			`item(
 *				[ `id( string  ),]
 *				string
 *				[ , true | false ]
 *				[ , itemList ]
 *			)
 *              </code>
 *
 *		The boolean parameter inside `item() indicates whether or not
 *		the respective tree item should be opened by default - if it
 *		has any subitems and if the respective UI is capable of closing
 *		and opening subtrees. If the UI cannot handle this, all
 *		subtrees will always be open.
 *
 * @option	multiSelection	user can select multiple items at once 
 * @option	immediate	make `notify trigger immediately when the selected item changes
 * @usage	`Tree( `id( `treeID ), "treeLabel", [ "top1", "top2", "top3" ] );
 * @examples	Tree1.ycp
 *		Tree2.ycp
 *		Tree3.ycp
 *		Tree-icons.ycp
 *		Tree-replace-items.ycp
 *		Wizard4.ycp
 *
 * @description
 *
 * A tree widget provides a selection from a hierarchical tree structure. The
 * semantics are very much like those of a SelectionBox. Unlike the
 * SelectionBox, however, tree items may have subitems that in turn may have
 * subitems etc.
 *
 * Each item has a label string, optionally preceded by an ID. If the item has
 * subitems, they are specified as a list of items after the string.
 *
 * The tree widget will not perform any sorting on its own: The items are
 * always sorted by insertion order. The application needs to handle sorting
 * itself, if desired.
 *
 * Note: The Qt version of the Wizard widget also provides a built-in tree with
 * an API that is (sometimes) easier to use.
 **/

YWidget *
YCPDialogParser::parseTree( YWidget * parent, YWidgetOpt & opt,
			    const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numArgs = term->size() - argnr;

    if ( numArgs < 1 || numArgs > 3
	 || ! term->value( argnr )->isString()
	 || ( numArgs >= 2 && ! term->value( argnr+1 )->isList() ) )
    {
	THROW_BAD_ARGS( term );
    }

    bool immediate  = false;
    bool multiSelection = false;
    bool recursiveSelection = false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_immediate )	          immediate  = true;
        else if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_multiSelection )     multiSelection = true;
        else if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_recursiveSelection ) recursiveSelection = true;
	else logUnknownOption( term, optList->value(o) );
    }

    string label = term->value ( argnr )->asString()->value();

    YTree * tree = YUI::widgetFactory()->createTree( parent, label, multiSelection, recursiveSelection );

    if ( numArgs > 1 )
    {
	YCPList itemList = term->value( argnr+1 )->asList();
	tree->addItems( YCPTreeItemParser::parseTreeItemList( itemList ) );

	if ( tree->hasItems() && !multiSelection )
	    tree->selectItem( tree->firstItem() );
    }

    if ( immediate )
	tree->setImmediateMode( true ); // includes setNotify()
    
    return tree;
}





/**
 * @widget	Table
 * @short	Multicolumn table widget
 * @class	YTable
 * @arg		term header the headers of the columns
 * @optarg	list items the items contained in the selection box
 * @option	immediate make `notify trigger immediately when the selected item changes
 * @option	keepSorting keep the insertion order - don't let the user sort manually by clicking
 * @option	multiSelection	user can select multiple items (rows) at once (shift-click, ctrl-click)
 * @usage	`Table( `header( "Game", "Highscore" ), [ `item( `id(1), "xkobo", "1708" ) ] )
 * @examples	Table1.ycp Table2.ycp Table3.ycp Table4.ycp Table5.ycp
 *
 * @description
 *
 * The Table widget is a selection list with multiple columns. By default, the user can
 * select exactly one row (with all its columns) from that list. With
 * `opt(`multiSelection), the user can select one or more rows (with all their
 * columns) from that list (In that case, use the `SelectedItems property, not
 * `Value).
 *
 * Each cell (each column within each row) has a label text and an optional
 * icon.
 *
 * (Note: Not all UIs (in particular not text-based UIs) support displaying
 * icons, so an icon should never be an exclusive means to display any kind of
 * information).
 *
 * This widget is similar to SelectionBox, but it has several columns for each
 * item (each row). If just one column is desired, consider using SelectionBox
 * instead.
 *
 * Note: This is not something like a spread sheet, and it doesn't pretend or
 * want to be. Actions are performed on rows, not on individual cells (columns
 * within one row).
 *
 * The first argument (after `opt() and `id() which both are optional) is
 * `header() which specifies the column headers (and implicitly the number of
 * columns) and optionally the alignment for each column. Default alignment is
 * left.
 *
 * In the list of items, an ID is specified for each item. Each item can have
 * less cells (columns) than the table has columns (from `header()), in which
 * case any missing cells are assumed to be empty. If an item has more cells
 * than the table has columns, any extra cells are ignored.
 *
 * Each cell has a text label (which might also be an empty string) and
 * optionally an icon. If a cell has an icon, it has to be specified with
 * `cell(`icon("myiconname.png", "Label text")).
 *
 * A simple table is specified like this:
 *
 * `Table(`id(`players),
 *        `header("Nick", "Age", "Role"),
 *        [
 *         `item(`id("Bluebird"), "Bluebird,	18, 	"Scout"  ),
 *         `item(`id("Ozzz"    ), "Ozzz",	23, 	"Wizard" ),
 *         `item(`id("Wannabe" ), "Wannabe",	17 ),
 *         `item(`id("Coxxan"  ), "Coxxan",	26, 	"Warrior")
 *        ]
 *        )
 *
 * This will create a 3-column table. The first column ("Nick") and the third
 * column ("Role") will be left aligned. The second column ("Age") will be
 * right aligned. note that "Wannabe" doesn't have a Role. This field will be
 * empty.
 *
 * A table that uses icons is specified like this:
 *
 * `Table(`id(`players),
 *        `header("Nick", "Age", "Role"),
 *        [
 *         `item(`id("Bluebird"), "Bluebird, 18, `cell(`icon("scout.png"),    "Scout" )  ),
 *         `item(`id("Ozzz"    ), "Ozzz",    23,                              "Wizard"   ),
 *         `item(`id("Wannabe" ), "Wannabe", `cell(`icon("underage.png", 17 )            ),
 *         `item(`id("Coxxan"  ), "Coxxan",  `cell(`icon("oldman.png",   26 ), "Warrior" )
 *        ]
 *        )
 *
 * In this example, "Bluebird" has an additional icon in his "Role" column, and
 * "Wannabe" and "Coxxan" both have additional icons in their "Age" columns.
 **/

YWidget *
YCPDialogParser::parseTable( YWidget * parent, YWidgetOpt & opt,
			     const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numArgs = term->size() - argnr;
    if ( numArgs < 1 || numArgs > 2
	 || !term->value(argnr)->isTerm()
	 || term->value(argnr)->asTerm()->name() != YUISymbol_header
	 || (numArgs == 2 && !term->value(argnr+1)->isList()))
    {
	THROW_BAD_ARGS( term );
    }


    // Parse options

    bool immediate	= false;
    bool keepSorting	= false;
    bool multiSelection	= false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if	( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_immediate	    ) immediate	     = true;
	else if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_keepSorting    ) keepSorting    = true;
	else if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_multiSelection ) multiSelection = true;
	else logUnknownOption( term, optList->value(o) );
    }

    YCPTerm headerTerm	= term->value( argnr )->asTerm();

    YTable * table = YUI::widgetFactory()->createTable( parent, parseTableHeader( headerTerm ), multiSelection );

    if ( keepSorting )
	table->setKeepSorting( true );

    if ( immediate )
	table->setImmediateMode( true );


    if ( numArgs >= 2 ) // Fill table with items, if item list is specified
    {
	YCPList itemList = term->value( argnr+1 )->asList();
	table->addItems( YCPTableItemParser::parseTableItemList( itemList ) );
    }

    return table;
}



YTableHeader *
YCPDialogParser::parseTableHeader( const YCPTerm & headerTerm )
{
    YTableHeader * header = new YTableHeader();
    YUI_CHECK_NEW( header );

    for ( int i=0; i < headerTerm->size(); i++ )
    {
	YCPValue colHeader = headerTerm->value( i );

	if ( colHeader->isString() )
	{
	    header->addColumn( colHeader->asString()->value() );
	}
	else if ( colHeader->isTerm() )
	{
	    string		headerText;
	    YAlignmentType	alignment	= YAlignBegin;
	    YCPTerm		colHeaderTerm	= colHeader->asTerm();

	    if      ( colHeaderTerm->name() == YUISymbol_Left   ) alignment = YAlignBegin;
	    else if ( colHeaderTerm->name() == YUISymbol_Right  ) alignment = YAlignEnd;
	    else if ( colHeaderTerm->name() == YUISymbol_Center ) alignment = YAlignCenter;
	    else
	    {
		string msg = string( "Unknown table header alignment: " )
		    + colHeaderTerm->name().c_str();
		yuiError() << msg << endl;
		ycperror( "%s", msg.c_str() );
	    }

	    if ( colHeaderTerm->size() > 0 )
	    {
		if ( colHeaderTerm->value(0)->isString() )
		    headerText = colHeaderTerm->value(0)->asString()->value();
		else
		{
		    string msg = string( "Expected string for table header, not " )
			+ colHeaderTerm->value(0)->toString();

		    yuiError() << msg << endl;
		    ycperror( "%s", msg.c_str() );
		}

		if ( colHeaderTerm->size() > 1 )
		{
		    string msg = string( "Ignoring extra parameters of %s" )
			+ colHeaderTerm->toString();

		    yuiError() << msg << endl;
		    ycperror( "%s", msg.c_str() );
		}
	    }

	    header->addColumn( headerText, alignment );
	}
    }

    return header;
}



/**
 * @widget	ProgressBar
 * @short	Graphical progress indicator
 * @class	YProgressBar
 * @arg		string label the label describing the bar
 * @optarg	integer maxvalue the maximum value of the bar
 * @optarg	integer progress the current progress value of the bar
 * @usage	`ProgressBar( `id( `pb ), "17 of 42 Packages installed", 42, 17 )
 * @examples	ProgressBar1.ycp ProgressBar2.ycp
 *
 * @description
 *
 * A progress bar is a horizontal bar with a label that shows a progress
 * value. If you omit the optional parameter <tt>maxvalue</tt>, the maximum
 * value will be 100. If you omit the optional parameter <tt>progress</tt>, the
 * progress bar will set to 0 initially.
 *
 * If you don't know the number of total steps you might want to use the
 * BusyIndicator widget instead of ProgressBar.
 *
 **/

YWidget *
YCPDialogParser::parseProgressBar( YWidget * parent, YWidgetOpt & opt,
				   const YCPTerm & term, const YCPList & optList, int argnr )
{
    int s = term->size() - argnr;
    if ( s < 1
	 || s > 3
	 || (s >= 1 && !term->value(argnr)->isString())
	 || (s >= 2 && !term->value(argnr+1)->isInteger())
	 || (s >= 3 && !term->value(argnr+2)->isInteger()))
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    string  label        = term->value( argnr )->asString()->value();
    int	    maxValue     = 100;
    int	    initialValue = 0;

    if ( s >= 2 ) maxValue	= term->value( argnr+1 )->asInteger()->value();
    if ( s >= 3 ) initialValue	= term->value( argnr+2 )->asInteger()->value();

    YProgressBar * progressBar = YUI::widgetFactory()->createProgressBar( parent, label, maxValue );

    if ( initialValue > 0 )
	progressBar->setValue( initialValue );

    return progressBar;
}



/**
 * @widget	Image
 * @short	Pixmap image
 * @class	YImage
 * @arg		string imageFileName  file name (with path) of the image to display
 * @option	animated	show an animated image (MNG, animated GIF)
 * @option	scaleToFit	scale the pixmap so it fits the available space: zoom in or out as needed
 * @option	zeroWidth	make widget report a preferred width of 0
 * @option	zeroHeight 	make widget report a preferred height of 0
 * @example	Image1.ycp Image-animated.ycp Image-scaled.ycp
 *
 * @description
 *
 * Displays an image if the respective UI is capable of that.
 *
 * Use <tt>`opt( `zeroWidth )</tt> and / or <tt>`opt( `zeroHeight )</tt>
 * if the real size of the image widget is determined by outside factors, e.g. by the size
 * of neighboring widgets. With those options you can override the preferred size of
 * the image widget and make it show just a part of the image.  If more screen space is
 * available, more of the image is shown, if not, the layout engine doesn't complain about
 * the image widget not getting its preferred size.
 *
 * `opt( `scaleToFit ) scales the image to fit into the available space, i.e. the
 * image will be zoomed in or out as needed.
 *
 * This option implicitly sets `opt( `zeroWidth ) and `opt( zeroHeight ),
 * too since there is no useful default size for such an image. Use MinSize()
 * or other layout helpers to explicitly set a size on such a widget.
 *
 * Please note that setting both `opt( `tiled ) and `opt( `scaleToFit ) at once
 * doesn't make any sense.
 **/

YWidget *
YCPDialogParser::parseImage( YWidget * parent, YWidgetOpt & opt,
			     const YCPTerm & term, const YCPList & optList, int argnr )
{
    int argCount = term->size() - argnr;

    bool ok = ( argCount == 1 && term->value( argnr )->isString() );	// 1 string arg

    ok = ( ok || ( argCount == 2					// 2 string args (old style)
		   && term->value( argnr   )->isString()
		   && term->value( argnr+1 )->isString() ) );

    if ( ! ok )
    {
	THROW_BAD_ARGS( term );
    }

    string	imageFileName	= term->value( argnr )->asString()->value();
    bool 	zeroWidth 	= false;
    bool 	zeroHeight 	= false;
    bool 	animated 	= false;
    bool 	autoScale	= false;

    for ( int o=0; o < optList->size(); o++ )
    {
	if	( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_zeroWidth	)  zeroWidth  = true;
	else if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_zeroHeight )  zeroHeight = true;
	else if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_animated	)  animated   = true;
	else if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_scaleToFit )  autoScale  = true;
	else logUnknownOption( term, optList->value(o) );
    }

    if ( autoScale )
    {
	zeroWidth = true;
	zeroHeight = true;
    }

    YImage * image = YUI::widgetFactory()->createImage( parent, imageFileName, animated );

    if ( zeroWidth  )	image->setZeroSize( YD_HORIZ, true );
    if ( zeroHeight )	image->setZeroSize( YD_VERT , true );
    if ( autoScale  )	image->setAutoScale( true );

    return image;
}



/*
 * @widget	IntField
 * @short	Numeric limited range input field
 * @class	YIntField
 * @arg		string	label		Explanatory label above the input field
 * @arg		integer minValue	minimum value
 * @arg		integer maxValue	maximum value
 * @arg		integer initialValue	initial value
 * @usage	`IntField( "Percentage", 1, 100, 50 )
 *
 * @examples	IntField1.ycp IntField2.ycp
 *
 * @description
 *
 * A numeric input field for integer numbers within a limited range.
 * This can be considered a lightweight version of the
 * <link linkend="Slider_widget">Slider</link> widget, even as a replacement for
 * this when the specific UI doesn't support the Slider.
 * Remember it always makes sense to specify limits for numeric input, even if
 * those limits are very large (e.g. +/- MAXINT).
 *
 * Fractional numbers are currently not supported.
 *
 **/

YWidget *
YCPDialogParser::parseIntField( YWidget * parent, YWidgetOpt & opt,
				const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numArgs = term->size() - argnr;

    if ( numArgs != 4
	 || ! term->value(argnr  )->isString()
	 || ! term->value(argnr+1)->isInteger()
	 || ! term->value(argnr+2)->isInteger()
	 || ! term->value(argnr+3)->isInteger()
	 )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    string	label		= term->value( argnr   )->asString()->value();
    int 	minValue	= term->value( argnr+1 )->asInteger()->value();
    int 	maxValue	= term->value( argnr+2 )->asInteger()->value();
    int 	initialValue	= term->value( argnr+3 )->asInteger()->value();

    return YUI::widgetFactory()->createIntField( parent, label, minValue, maxValue, initialValue );
}



/*
 * @widget	PackageSelector
 * @short	Complete software package selection
 * @class	YPackageSelector
 * @optarg	string floppyDevice
 * @option	youMode start in YOU (YaST Online Update) mode
 * @option	updateMode start in update mode
 * @option	searchMode start with the "search" filter view
 * @option	summaryMode start with the "installation summary" filter view
 * @option	repoMode start with the "repositories" filter view
 * @option	repoMgr enable "Repository Manager" menu item
 * @option	confirmUnsupported user has to confirm all unsupported (non-L3) packages
 * @usage	`PackageSelector()
 *
 * @examples	PackageSelector.ycp
 *
 * @description
 *
 * A very complex widget that handles software package selection completely
 * transparently. Set up the package manager (the backend) before creating this
 * widget and let the package manager and the package selector handle all the
 * rest. The result of all this are the data stored in the package manager.
 *
 * Use UI::RunPkgSelection() after creating a dialog with this widget.
 * The result of UI::UserInput() in a dialog with such a widget is undefined -
 * it may or may not return.
 *
 * This widget gets the (best) floppy device as a parameter since the UI has no
 * general way of finding out by itself what device can be used for saving or
 * loading pacakge lists etc. - this is best done outside and passed here as a
 * parameter.
 *
 **/

YWidget *
YCPDialogParser::parsePackageSelector( YWidget * parent, YWidgetOpt & opt,
				       const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numArgs = term->size() - argnr;

    if ( numArgs > 1 ||
	 ( numArgs == 1 && ! term->value( argnr )->isString() ) )
    {
	THROW_BAD_ARGS( term );
    }


    // Parse options

    long modeFlags = 0;

    for ( int o=0; o < optList->size(); o++ )
    {
	if ( optList->value(o)->isSymbol() )
	{
	    string sym = optList->value(o)->asSymbol()->symbol();

	    if	    ( sym == YUIOpt_youMode     	) 	modeFlags |= YPkg_OnlineUpdateMode;
	    else if ( sym == YUIOpt_updateMode  	) 	modeFlags |= YPkg_UpdateMode;
	    else if ( sym == YUIOpt_searchMode  	) 	modeFlags |= YPkg_SearchMode;
	    else if ( sym == YUIOpt_summaryMode 	)	modeFlags |= YPkg_SummaryMode;
	    else if ( sym == YUIOpt_repoMode		)	modeFlags |= YPkg_RepoMode;
	    else if ( sym == YUIOpt_testMode 		)	modeFlags |= YPkg_TestMode;
	    else if ( sym == YUIOpt_repoMgr 		)	modeFlags |= YPkg_RepoMgr;
	    else if ( sym == YUIOpt_confirmUnsupported	)	modeFlags |= YPkg_ConfirmUnsupported;
	    else logUnknownOption( term, optList->value(o) );
	}
	else logUnknownOption( term, optList->value(o) );
    }

    return YUI::widgetFactory()->createPackageSelector( parent, modeFlags );
}



/*
 * @widget	PkgSpecial
 * @short	Package selection special - DON'T USE IT
 * @class	YPkgSpecial
 * @usage	`PkgSpecial( "subwidget_name" )
 *
 * @description
 *
 * Use only if you know what you are doing - that is, DON'T USE IT.
 *
 **/

YWidget *
YCPDialogParser::parsePkgSpecial( YWidget * parent, YWidgetOpt & opt,
				  const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numArgs = term->size() - argnr;

    if ( numArgs != 1
	 || ! term->value( argnr )->isString() )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );
    string subwidgetName = term->value( argnr )->asString()->value();

    return YUI::widgetFactory()->createPkgSpecial( parent, subwidgetName );
}



// =============================================================================
//			       Optional Widgets
// =============================================================================


YWidget *
YCPDialogParser::parseDummySpecialWidget( YWidget *parent, YWidgetOpt & opt,
					  const YCPTerm & term, const YCPList & optList, int argnr )
{
    if ( term->size() - argnr > 0 )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term, optList );

    return YUI::optionalWidgetFactory()->createDummySpecialWidget( parent );
}


// ----------------------------------------------------------------------

/*
 * @widget	BarGraph
 * @short	Horizontal bar graph (optional widget)
 * @class	YBarGraph
 * @arg		list values the initial values (integer numbers)
 * @optarg	list labels the labels for each part; use "%1" to include the
 *		current numeric value. May include newlines.
 * @usage	if ( HasSpecialWidget( `BarGraph ) {...
 *		`BarGraph( [ 450, 100, 700 ],
 *		[ "Windows used\n%1 MB", "Windows free\n%1 MB", "Linux\n%1 MB" ] )
 *
 * @examples	BarGraph1.ycp BarGraph2.ycp BarGraph3.ycp
 *
 * @description
 *
 * A horizontal bar graph for graphical display of proportions of integer
 * values.  Labels can be passed for each portion; they can include a "%1"
 * placeholder where the current value will be inserted (sformat() -style) and
 * newlines. If no labels are specified, only the values will be
 * displayed. Specify empty labels to suppress this.
 * @note        This is a "special" widget, i.e. not all UIs necessarily support it.  Check
 * for availability with <tt>HasSpecialWidget( `BarGraph )</tt> before using it.
 *
 **/

YWidget *
YCPDialogParser::parseBarGraph( YWidget *parent, YWidgetOpt & opt,
				const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numArgs = term->size() - argnr;

    if ( numArgs < 1 || numArgs > 2
	 || ! term->value(argnr)->isList()
	 || ( numArgs > 1 && ! term->value(argnr+1)->isList() )
	 )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    YBarGraph * barGraph = YUI::optionalWidgetFactory()->createBarGraph( parent );
    YBarGraphMultiUpdate multiUpdate( barGraph ); // Hold back display updates

    YCPList valuesList = term->value( argnr )->asList();
    YCPList labelsList;

    if ( numArgs > 1 )
	labelsList = term->value( argnr+1 )->asList();

    int segments = valuesList->size();

    if ( labelsList->size() > segments )
	segments = labelsList->size();

    for ( int i=0; i < segments; i++ )
    {
	int value = 0;

	if ( i < valuesList->size() )
	{
	    if ( valuesList->value(i)->isInteger() )
		value = valuesList->value(i)->asInteger()->value();
	    else
	    {
		ycperror( "YBarGraph value #%d should be integer, not %s",
			  i, valuesList->value(i)->toString().c_str() );
	    }
	}

	string label;

	if ( i < labelsList->size() )
	{
	    if ( labelsList->value(i)->isString() )
		label = labelsList->value(i)->asString()->value();
	    else
	    {
		ycperror( "YBarGraph label #%d should be integer, not %s",
			  i, labelsList->value(i)->toString().c_str() );
	    }
	}

	barGraph->addSegment( YBarGraphSegment( value, label ) );
    }

    return barGraph;
}

// ----------------------------------------------------------------------


/*
 * @widget	DownloadProgress
 * @short	Self-polling file growth progress indicator (optional widget)
 * @class	YDownloadProgress
 * @arg		string label label above the indicator
 * @arg		string filename file name with full path of the file to poll
 * @arg		integer expectedSize expected final size of the file in bytes
 * @usage	if ( HasSpecialWidget( `DownloadProgress ) {...
 *		`DownloadProgress( "Base system (230k)", "/tmp/aaa_base.rpm", 230*1024 );
 *
 * @examples	DownloadProgress1.ycp
 *
 * @description
 *
 * This widget automatically displays the progress of a lengthy download
 * operation. The widget itself (i.e. the UI) polls the specified file and
 * automatically updates the display as required even if the download is taking
 * place in the foreground.
 *
 * @note This is a "special" widget, i.e. not all UIs necessarily support it.  Check
 * for availability with <tt>HasSpecialWidget( `DownloadProgress )</tt> before using it.
 *
 **/

YWidget *
YCPDialogParser::parseDownloadProgress( YWidget *parent, YWidgetOpt & opt,
					const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numArgs = term->size() - argnr;

    if ( numArgs != 3
	 || ! term->value(argnr  )->isString()
	 || ! term->value(argnr+1)->isString()
	 || ! term->value(argnr+2)->isInteger()
	 )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    string	label	 	= term->value( argnr   )->asString()->value();
    string	filename 	= term->value( argnr+1 )->asString()->value();
    YFileSize_t	expectedSize	= term->value( argnr+2 )->asInteger()->value();

    return YUI::optionalWidgetFactory()->createDownloadProgress( parent, label, filename, expectedSize );
}



// ----------------------------------------------------------------------

/*
 * @widget	DumbTab
 * @short	Simplistic tab widget that behaves like push buttons
 * @class	YDumbTab
 * @arg		list tabs page headers
 * @arg		term contents page contents - usually a ReplacePoint
 * @usage	if ( HasSpecialWidget( `DumbTab) {...
 *		`DumbTab( [ `item(`id(`page1), "Page &1" ), `item(`id(`page2), "Page &2" ) ], contents; }
 *
 * @examples	DumbTab1.ycp DumbTab2.ycp
 *
 * @description
 *
 * This is a very simplistic approach to tabbed dialogs: The application
 * specifies a number of tab headers and the page contents and takes care of
 * most other things all by itself, in particular page switching. Each tab
 * header behaves very much like a PushButton - as the user activates a tab
 * header, the DumbTab widget simply returns the ID of that tab (or its text if
 * it has no ID). The application should then take care of changing the page
 * contents accordingly - call UI::ReplaceWidget() on the ReplacePoint
 * specified as tab contents or similar actions (it might even just replace
 * data in a Table or RichText widget if this is the tab contents). Hence the
 * name <i>Dumb</i>Tab.
 *
 * The items in the item list can either be simple strings or `item() terms
 * with an optional ID for each individual item (which will be returned upon
 * UI::UserInput() and related when the user selects this tab), a (mandatory)
 * user-visible label and an (optional) flag that indicates that this tab is
 * initially selected. If you specify only a string, UI::UserInput() will
 * return this string.
 *
 * This is a "special" widget, i.e. not all UIs necessarily support it.  Check
 * for availability with <tt>HasSpecialWidget( `DumbTab )</tt> before
 * using it.
 *
 * @note Please notice that using this kind of widget more often than not is the
 * result of <b>poor dialog or workflow design</b>.
 *
 * Using tabs only hides complexity, but the complexity remains there. They do
 * little to make problems simpler. This however should be the approach of
 * choice for good user interfaces.
 *
 * It is very common for tabs to be overlooked by users if there are just two
 * tabs to select from, so in this case better use an "Expert..." or
 * "Details..." button - this gives much more clue to the user that there is
 * more information  available while at the same time clearly indicating that
 * those other options are much less commonly used.
 *
 * If there are very many different views on data or if there are lots and lots
 * of settings, you might consider using a tree for much better navigation. The
 * Qt UI's wizard even has a built-in tree that can be used instead of the help
 * panel.
 *
 * If you use a tree for navigation, unter all circumstances avoid using tabs
 * at the same time - there is no way for the user to tell which tree nodes
 * have tabs and which have not, making navigation even more difficult.
 * KDE's control center or Mozilla's settings are very good examples
 * how <b>not</b> to do that - you become bogged down for sure in all those
 * tree nodes and tabs hidden within so many of them.
 *
 **/

YWidget *
YCPDialogParser::parseDumbTab( YWidget *parent, YWidgetOpt & opt,
			       const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numArgs = term->size() - argnr;

    if ( numArgs != 2
	 || ! term->value( argnr   )->isList()
	 || ! term->value( argnr+1 )->isTerm()
	 )
    {
	THROW_BAD_ARGS( term );
    }

    YCPList itemList  = term->value( argnr   )->asList();
    YCPTerm childTerm = term->value( argnr+1 )->asTerm();

    rejectAllOptions( term,optList );

    YDumbTab * dumbTab = YUI::optionalWidgetFactory()->createDumbTab( parent );
    dumbTab->addItems( YCPItemParser::parseItemList( itemList ) ); // Add tab pages
    parseWidgetTreeTerm( dumbTab, childTerm );

    return dumbTab;
}



/*
 * @widget	VMultiProgressMeter HMultiProgressMeter
 * @short	Progress bar with multiple segments (optional widget)
 * @class	YMultiProgressMeter
 * @arg		List<integer> maxValues		maximum values
 * @usage	if ( HasSpecialWidget( `MultiProgressMeter ) {...
 *		`MultiProgressMeter( "Percentage", 1, 100, 50 )
 *
 * @examples	MultiProgressMeter1.ycp MultiProgressMeter2.ycp
 *
 * @description
 *
 * A vertical (VMultiProgressMeter) or horizontal (HMultiProgressMeter)
 * progress display with multiple segments. The numbers passed on widget
 * creation are the maximum numbers of each individual segment. Segments sizes
 * will be displayed proportionally to these numbers.
 *
 * This widget is intended for applications like showing the progress of
 * installing from multiple CDs while giving the user a hint how much will be
 * installed from each individual CD.
 *
 * Set actual values later with
 * <code>
 * UI::ChangeWidget(`id(...), `Values, [ 1, 2, ...] );
 * </code>
 *
 * The widget may choose to reserve a minimum amount of space for each segment
 * even if that means that some segments will be shown slightly out of
 * proportion.
 *
 * @note  This is a "special" widget, i.e. not all UIs necessarily support it.  Check
 * for availability with <tt>HasSpecialWidget( `MultiProgressMeter )</tt> before using it.
 **/

YWidget *
YCPDialogParser::parseMultiProgressMeter( YWidget *parent, YWidgetOpt & opt,
					  const YCPTerm & term, const YCPList & optList, int argnr,
					  YUIDimension dim )
{
    int numArgs = term->size() - argnr;

    if ( numArgs != 1 || ! term->value(argnr)->isList() )
    {
	THROW_BAD_ARGS( term );
    }

    vector<float> maxValues;

    try
    {
	maxValues = parseNumList( term->value( argnr )->asList() );
    }
    catch ( YUIException & exception )
    {
	YUI_RETHROW( exception );
    }

    if ( maxValues.size() < 1 )
    {
	YUI_THROW( YCPDialogSyntaxErrorException( "Expected list<integer> or list<float>",
						  term->value( argnr ) ) );
    }

    rejectAllOptions( term,optList );

    return YUI::optionalWidgetFactory()->createMultiProgressMeter( parent, dim, maxValues );
}


vector<float>
YCPDialogParser::parseNumList( const YCPList & yList )
{
    vector<float> result;

    for ( int i=0; i < yList->size(); i++ )
    {
	YCPValue val = yList->value( i );

	if ( val->isInteger() )
	{
	    result.push_back( (float) val->asInteger()->value() );
	}
	else if ( val->isFloat() )
	{
	    result.push_back( val->asFloat()->value() );
	}
	else
	{
	    YUI_THROW( YCPDialogSyntaxErrorException( "Expected list<integer> or list<float>", yList ) );
	}
    }

    return result;
}



/*
 * @widget	Slider
 * @short	Numeric limited range input (optional widget)
 * @class	YSlider
 * @arg		string	label		Explanatory label above the slider
 * @arg		integer minValue	minimum value
 * @arg		integer maxValue	maximum value
 * @arg		integer initialValue	initial value
 * @usage	if ( HasSpecialWidget( `Slider ) {...
 *		`Slider( "Percentage", 1, 100, 50 )
 *
 * @examples	Slider1.ycp Slider2.ycp
 *
 * @description
 * A horizontal slider with (numeric) input field that allows input of an
 * integer value in a given range. The user can either drag the slider or
 * simply enter a value in the input field.
 *
 * Remember you can use <tt>`opt( `notify )</tt> in order to get instant response
 * when the user changes the value - if this is desired.
 *
 * @note  This is a "special" widget, i.e. not all UIs necessarily support it.  Check
 * for availability with <tt>HasSpecialWidget( `Slider )</tt> before using it.
 *
 **/

YWidget *
YCPDialogParser::parseSlider( YWidget *parent, YWidgetOpt & opt,
			      const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numArgs = term->size() - argnr;

    if ( numArgs != 4
	 || ! term->value(argnr)->isString()
	 || ! term->value(argnr+1)->isInteger()
	 || ! term->value(argnr+2)->isInteger()
	 || ! term->value(argnr+3)->isInteger()
	 )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    string	label		= term->value( argnr   )->asString()->value();
    int 	minValue	= term->value( argnr+1 )->asInteger()->value();
    int 	maxValue	= term->value( argnr+2 )->asInteger()->value();
    int 	initialValue	= term->value( argnr+3 )->asInteger()->value();

    return YUI::optionalWidgetFactory()->createSlider( parent, label, minValue, maxValue, initialValue );
}


/*
 * @widget	PartitionSplitter
 * @short	Hard disk partition splitter tool (optional widget)
 * @class	YPartitionSplitter
 *
 * @arg integer	usedSize		size of the used part of the partition
 * @arg integer	totalFreeSize 		total size of the free part of the partition
 *					(before the split)
 * @arg integer newPartSize		suggested size of the new partition
 * @arg integer minNewPartSize		minimum size of the new partition
 * @arg integer minFreeSize		minimum free size of the old partition
 * @arg string	usedLabel 		BarGraph label for the used part of the old partition
 * @arg string	freeLabel 		BarGraph label for the free part of the old partition
 * @arg string	newPartLabel  		BarGraph label for the new partition
 * @arg string	freeFieldLabel		label for the remaining free space field
 * @arg string	newPartFieldLabel	label for the new size field
 * @usage	if ( HasSpecialWidget( `PartitionSplitter ) {...
 *		`PartitionSplitter( 600, 1200, 800, 300, 50,
 *                                 "Windows used\n%1 MB", "Windows used\n%1 MB", "Linux\n%1 MB", "Linux ( MB )" )
 *
 * @examples	PartitionSplitter1.ycp PartitionSplitter2.ycp
 *
 * @description
 *
 * A very specialized widget to allow a user to comfortably split an existing
 * hard disk partition in two parts. Shows a bar graph that displays the used
 * space of the partition, the remaining free space (before the split) of the
 * partition and the space of the new partition (as suggested).
 * Below the bar graph is a slider with an input fields to the left and right
 * where the user can either input the desired remaining free space or the
 * desired size of the new partition or drag the slider to do this.
 *
 * The total size is <tt>usedSize+freeSize</tt>.
 *
 * The user can resize the new partition between <tt>minNewPartSize</tt> and
 * <tt>totalFreeSize-minFreeSize</tt>.
 *
 * @note This is a "special" widget, i.e. not all UIs necessarily support it.  Check
 * for availability with <tt>HasSpecialWidget( `PartitionSplitter )</tt> before using it.
 **/

YWidget *
YCPDialogParser::parsePartitionSplitter( YWidget *parent, YWidgetOpt & opt,
					 const YCPTerm & term, const YCPList & optList, int argnr )
{
    int numArgs = term->size() - argnr;

    if ( numArgs != 10
	 || ! term->value(argnr  )->isInteger()	// usedSize
	 || ! term->value(argnr+1)->isInteger()	// freeSize
	 || ! term->value(argnr+2)->isInteger()	// newPartSize
	 || ! term->value(argnr+3)->isInteger()	// minNewPartSize
	 || ! term->value(argnr+4)->isInteger()	// minFreeSize
	 || ! term->value(argnr+5)->isString()	// usedLabel
	 || ! term->value(argnr+6)->isString()	// freeLabel
	 || ! term->value(argnr+7)->isString()	// newPartLabel
	 || ! term->value(argnr+8)->isString()	// freeFieldLabel
	 || ! term->value(argnr+9)->isString()	// newPartFieldLabel
	 )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term, optList );

    int 	usedSize		= term->value( argnr   )->asInteger()->value();
    int 	totalFreeSize		= term->value( argnr+1 )->asInteger()->value();
    int 	newPartSize		= term->value( argnr+2 )->asInteger()->value();
    int 	minNewPartSize		= term->value( argnr+3 )->asInteger()->value();
    int 	minFreeSize		= term->value( argnr+4 )->asInteger()->value();
    string 	usedLabel		= term->value( argnr+5 )->asString()->value();
    string 	freeLabel		= term->value( argnr+6 )->asString()->value();
    string 	newPartLabel		= term->value( argnr+7 )->asString()->value();
    string 	freeFieldLabel		= term->value( argnr+8 )->asString()->value();
    string 	newPartFieldLabel	= term->value( argnr+9 )->asString()->value();

    return YUI::optionalWidgetFactory()->createPartitionSplitter( parent,
								  usedSize,
								  totalFreeSize,
								  newPartSize,
								  minNewPartSize,
								  minFreeSize,
								  usedLabel,
								  freeLabel,
								  newPartLabel,
								  freeFieldLabel,
								  newPartFieldLabel );
}



/*
 * @widget	PatternSelector
 * @short	High-level widget to select software patterns (selections)
 * @class	YPatternSelector
 * @usage	if ( UI::HasSpecialWidget( `PatternSelector) {...
 *		`PatternSelector()...
 *		UI::RunPkgSelection();
 *
 * @examples	PatternSelector-solo.ycp PatternSelector-wizard.ycp
 *
 * @description
 *
 * This widget is similar to the PackageSelector in its semantics: It is a very
 * high-level widget that lets the user select software, but unlike the
 * PackageSelector it works on software patterns (selections).
 **/

YWidget *
YCPDialogParser::parsePatternSelector( YWidget *parent, YWidgetOpt & opt,
				       const YCPTerm & term, const YCPList & optList, int argnr )
{
    if ( term->size() - argnr > 0 )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term, optList );

    return YUI::optionalWidgetFactory()->createPatternSelector( parent );
}


/*
 * @widget	SimplePatchSelector
 * @short	Simplified approach to patch selection
 * @class	YSimplePatchSelector
 * @usage	if ( UI::HasSpecialWidget( `SimplePatchSelector) {...
 *		`SimplePatchSelector()...
 *		UI::RunPkgSelection();
 *
 * @examples	SimplePatchSelector-empty.ycp SimplePatchSelector-stable.ycp
 *
 * @description
 *
 * This is a stripped-down version of the PackageSelector widget in "online
 * update" ("patches") mode. It provides a very simplistic view on patches. It does
 * not give access to handling packages by itself, but it contains a
 * "Details..." button that lets the application open a full-fledged
 * PackageSelector (in "online update" / "patches" mode).
 *
 * Be advised that only this widget alone without access to the full
 * PackageSelector might easily lead the user to a dead end: If dependency
 * problems arise that cannot easily be solved from within the dependency
 * problems dialog or by deselecting one or several patches, it might be
 * necessary for the user to solve the problem on the package level. If he
 * cannot do that, he might be unable to continue his update task.
 *
 * This widget is similar in many ways to the PatternSelector widget: It gives
 * a higher-level, more abstract access to package management at the cost of
 * omitting details and fine control that more advanced users will want or
 * need. The SimplePatchSelector should be used in ways similarl to the
 * PatternSelector widget.
 **/

YWidget *
YCPDialogParser::parseSimplePatchSelector( YWidget *parent, YWidgetOpt & opt,
					   const YCPTerm & term, const YCPList & optList, int argnr )
{
    if ( term->size() - argnr > 0 )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term, optList );

    return YUI::optionalWidgetFactory()->createSimplePatchSelector( parent );
}



/**
 * @widgets	DateField
 * @short	Date input field
 * @class	YDateField
 * @arg		string label
 * @optarg	string initialDate
 * @usage	if ( HasSpecialWidget( `DateField ) {...
 *		    `DateField( "Date:", "2004-10-12" )
 *
 * @description
 * An input field for entering a date.
 *
 * @note This is a "special" widget, i.e. not all UIs necessarily support it.  Check
 * for availability with <tt>HasSpecialWidget( `TimeField)</tt> before using it.
 **/

YWidget *
YCPDialogParser::parseDateField( YWidget * parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr )
{

    if ( term->size() - argnr < 1 || term->size() - argnr > 2
	 || !term->value(argnr)->isString()
	 || (term->size() == argnr+2 && !term->value(argnr+1)->isString()))
    {
	THROW_BAD_ARGS( term );
    }


    rejectAllOptions( term,optList );

    string label = term->value( argnr )->asString()->value();

    YDateField * dateField = YUI::optionalWidgetFactory()->createDateField( parent, label );

    if ( term->size() >= argnr + 2 )
    {
	string initialValue = term->value( argnr+1 )->asString()->value();
	dateField->setValue( initialValue );
    }

    return dateField;
}



/**
 * @widgets	TimeField
 * @short	Time input field
 * @class	YTimeField
 * @arg		string label
 * @optarg	string initialTime
 * @usage	if ( HasSpecialWidget( `TimeField ) {...
 * 		    `TimeField( "Time:" , "20:20:20" )
 *
 * @description
 * An input field for entering a time of day in 24 hour format.
 *
 * @note This is a "special" widget, i.e. not all UIs necessarily support it.  Check
 * for availability with <tt>HasSpecialWidget( `TimeField)</tt> before using it.
 **/
YWidget *
YCPDialogParser::parseTimeField( YWidget * parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr )
{

    if ( term->size() - argnr < 1 || term->size() - argnr > 2
	 || !term->value(argnr)->isString()
	 || (term->size() == argnr+2 && !term->value(argnr+1)->isString()))
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    string label = term->value( argnr )->asString()->value();

    YTimeField * timeField = YUI::optionalWidgetFactory()->createTimeField( parent, label );

    if ( term->size() >= argnr + 2 )
    {
	string initialValue = term->value( argnr+1 )->asString()->value();
	timeField->setValue( initialValue );
    }

    return timeField;
}



/*
 * @widget	Wizard
 * @short	Wizard frame - not for general use, use the Wizard:: module instead!
 * @class	YWizard
 *
 * @option	stepsEnabled	Enable showing wizard steps (use UI::WizardCommand() to set them).
 * @option	treeEnabled	Enable showing a selection tree in the left panel. Disables stepsEnabled.
 *
 * @arg		any	backButtonId		ID to return when the user presses the "Back" button
 * @arg		string	backButtonLabel		Label of the "Back" button
 *
 * @arg		any	abortButtonId		ID to return when the user presses the "Abort" button
 * @arg		string	abortButtonLabel	Label of the "Abort" button
 *
 * @arg		any	nextButtonId		ID to return when the user presses the "Next" button
 * @arg		string	nextButtonLabel		Label of the "Next" button
 *
 * @usage	`Wizard(`id(`back), "&Back", `id(`abort), "Ab&ort", `id(`next), "&Next" )
 * @usage	`Wizard(`back, "&Back", `abort, "Ab&ort", `next, "&Next" )
 *
 * @description
 *
 * This is the UI-specific technical implementation of a wizard dialog's main widget.
 * This is not intended for general use - use the Wizard:: module instead which will use this
 * widget properly.
 *
 * A wizard widget always has ID `wizard.
 * The ID of the single replace point within the wizard is always `contents.
 *
 * @note This is a "special" widget, i.e. not all UIs necessarily support it.  Check
 * for availability with <tt>HasSpecialWidget( `Wizard)</tt> before using it.
 **/

YWidget *
YCPDialogParser::parseWizard( YWidget * parent, YWidgetOpt & opt,
			      const YCPTerm & term, const YCPList & optList, int argnr )
{
    if ( term->size() - argnr != 6
	 || ! isSymbolOrId( term->value( argnr   ) ) || ! term->value( argnr+1 )->isString()
	 || ! isSymbolOrId( term->value( argnr+2 ) ) || ! term->value( argnr+3 )->isString()
	 || ! isSymbolOrId( term->value( argnr+4 ) ) || ! term->value( argnr+5 )->isString() )
    {
	THROW_BAD_ARGS( term );
    }


    // Parse options

    YWizardMode wizardMode = YWizardMode_Standard;

    for ( int o=0; o < optList->size(); o++ )
    {
	if      ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_stepsEnabled ) wizardMode = YWizardMode_Steps;
	else if ( optList->value(o)->isSymbol() && optList->value(o)->asSymbol()->symbol() == YUIOpt_treeEnabled  ) wizardMode = YWizardMode_Tree;
	else logUnknownOption( term, optList->value(o) );
    }

    YWidgetID *	backButtonId		= new YCPValueWidgetID( parseIdTerm( term->value( argnr ) ) );
    string	backButtonLabel 	= term->value( argnr+1 )->asString()->value();

    YWidgetID *	abortButtonId		= new YCPValueWidgetID( parseIdTerm( term->value( argnr+2 ) ) );
    string	abortButtonLabel	= term->value( argnr+3 )->asString()->value();

    YWidgetID *	nextButtonId		= new YCPValueWidgetID( parseIdTerm( term->value( argnr+4 ) ) );
    string	nextButtonLabel		= term->value( argnr+5 )->asString()->value();

    YWizard * wizard =
	YUI::optionalWidgetFactory()->createWizard( parent,
						    backButtonLabel,
						    abortButtonLabel,
						    nextButtonLabel,
						    wizardMode );
    YUI_CHECK_NEW( wizard );

    // All wizard widgets have a fixed ID `wizard
    YWidgetID * wizardId = new YCPValueWidgetID( YCPSymbol( YWizardID ) );
    wizard->setId( wizardId );

    // The wizard internal contents ReplacePoint has a fixed ID `contents
    YWidgetID * contentsId =  new YCPValueWidgetID( YCPSymbol( YWizardContentsReplacePointID ) );

    if ( wizard->backButton()  ) 		wizard->backButton()->setId ( backButtonId  );
    if ( wizard->abortButton() ) 		wizard->abortButton()->setId( abortButtonId );
    if ( wizard->nextButton()  ) 		wizard->nextButton()->setId ( nextButtonId  );
    wizard->contentsReplacePoint()->setId( contentsId );

    return wizard;
}

/**
 * @widgets	TimezoneSelector
 * @short	Timezone selector map
 * @class	YTimezoneSelector
 *
 * @arg		string pixmap     path to a jpg or png of a world map - with 0°0° being the
 *                                middle of the picture
 * @arg         map timezones     a map of timezones. The map should be between e.g. Europe/London
 *                                and the tooltip to be displayed ("United Kingdom")
 *
 * @usage	if ( HasSpecialWidget( `TimezoneSelector ) {...
 * 		    `TimezoneSelector( "world.jpg", timezones )
 *
 * @description
 * An graphical timezone selector map
 *
 * @note This is a "special" widget, i.e. not all UIs necessarily support it.  Check
 * for availability with <tt>HasSpecialWidget( `TimezoneSelector)</tt> before using it.
 **/
YWidget *
YCPDialogParser::parseTimezoneSelector( YWidget * parent, YWidgetOpt & opt,
                                        const YCPTerm & term, const YCPList & optList, int argnr )
{
    if ( term->size() - argnr != 2
	 || !term->value(argnr)->isString()
         || !term->value(argnr+1)->isMap() )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term, optList );

    string pixmap = term->value( argnr )->asString()->value();
    map<string,string> zones;
    YCPMap secondArg = term->value( argnr+1 )->asMap();
    for ( YCPMapIterator it = secondArg.begin(); it != secondArg.end(); ++it )
        zones[ it.key()->asString()->value() ] = it.value()->asString()->value();

    YTimezoneSelector * selector = YUI::optionalWidgetFactory()->createTimezoneSelector( parent, pixmap, zones );

    return selector;
}


/**
 * @widgets	Graph
 * @short	graph
 * @class	YGraph
 *
 * @usage	if ( HasSpecialWidget( `Graph ) {...
 * 		    `Graph( "graph.dot", "dot" )
 *
 * @description
 * An graph
 *
 * @note This is a "special" widget, i.e. not all UIs necessarily support it.  Check
 * for availability with <tt>HasSpecialWidget(`Graph)</tt> before using it.
 **/
YWidget *
YCPDialogParser::parseGraph( YWidget * parent, YWidgetOpt & opt,
			     const YCPTerm & term, const YCPList & optList, int argnr )
{
    if ( term->size() - argnr != 2
	 || !term->value(argnr)->isString()
	 || !term->value(argnr+1)->isString() )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term, optList );

    string filename = term->value( argnr )->asString()->value();
    string layoutAlgorithm = term->value( argnr+1 )->asString()->value();

    YGraph * graph = YUI::optionalWidgetFactory()->createGraph( parent, filename, layoutAlgorithm );

    return graph;
}


/**
 * @widget	BusyIndicator
 * @short	Graphical busy indicator
 * @class	YBusyIndicator
 * @arg		string	label	the label describing the bar
 * @optarg	integer	timeout	the timeout in milliseconds until busy indicator changes to stalled state, 1000ms by default
 * @usage	`BusyIndicator(`id(`busy), "background action", 2000 ),
 * @example	BusyIndicator.ycp
 *
 * @description
 *
 * A busy indicator is a bar with a label that gives feedback to the user that
 * a task is in progress and the user has to wait. It is similar to a progress bar.
 * The difference is that a busy indicator can be used when the total number of
 * steps is not known before the action starts. You have to send keep alive messages
 * by setting alive to true every now and then, otherwise the busy indicator will
 * change to stalled state.
 *
 * There are some limitations due to technical reasons in ncurses ui:
 * Only one BusyIndicator widget works at the same time.
 * The BusyIndicator widget cannot be used together with an UserInput widget.
 * Please use the TimeoutUserInput widget in a loop instead.
 **/

YWidget *
YCPDialogParser::parseBusyIndicator( YWidget * parent, YWidgetOpt & opt,
				   const YCPTerm & term, const YCPList & optList, int argnr )
{
    int s = term->size() - argnr;
    if ( s < 1
	 || s > 2
	 || (s >= 1 && !term->value(argnr)->isString())
	 || (s >= 2 && !term->value(argnr+1)->isInteger()) )
    {
	THROW_BAD_ARGS( term );
    }

    rejectAllOptions( term,optList );

    string  label        = term->value( argnr )->asString()->value();
    int	    timeout      = 1000;

    if ( s >= 2 ) timeout	= term->value( argnr+1 )->asInteger()->value();

    YBusyIndicator * busyIndicator = YUI::widgetFactory()->createBusyIndicator( parent, label, timeout );

    return busyIndicator;
}




// =============================================================================
//			       Helper Functions
// =============================================================================



YWidget *
YCPDialogParser::findWidgetWithId( const YCPValue & idVal, bool doThrow )
{
    YDialog * dialog = YDialog::currentDialog( doThrow );

    if ( dialog )
	return findWidgetWithId( dialog, idVal, doThrow );
    else
	return 0;
}


YWidget *
YCPDialogParser::findWidgetWithId( YWidget * widgetRoot, const YCPValue & idVal, bool doThrow )
{
    YUI_CHECK_PTR( widgetRoot );

    YCPValueWidgetID id( idVal );
    YWidget * widget = widgetRoot->findWidget( &id, doThrow );

    return widget;
}


bool
YCPDialogParser::checkId( const YCPValue & v, bool complain )
{
    if ( v->isTerm()
	 && v->asTerm()->size() == 1
	 && v->asTerm()->name() == YUISymbol_id ) return true;
    else
    {
	if ( complain )
	{
	    ycperror( "Expected `" YUISymbol_id "( any v ), not  %s", v->toString().c_str() );
	}
	return false;
    }
}


bool
YCPDialogParser::isSymbolOrId( const YCPValue & val )
{
    if ( val->isTerm()
	 && val->asTerm()->name() == YUISymbol_id )
    {
	return ( val->asTerm()->size() == 1 );
    }

    return val->isSymbol();
}


YCPValue
YCPDialogParser::parseIdTerm( const YCPValue & val )
{
    if ( val->isTerm() && val->asTerm()->name() == YUISymbol_id )
	return val->asTerm()->value(0);

    return val;
}


YCPValue
YCPDialogParser::getWidgetId( const YCPTerm & term, int *argnr )
{
    if ( term->size() > 0
	 && term->value(0)->isTerm()
	 && term->value(0)->asTerm()->name() == YUISymbol_id )
    {
	YCPTerm idterm = term->value(0)->asTerm();
	if ( idterm->size() != 1 )
	{
	    ycperror( "Widget id `" YUISymbol_id "() expects exactly one argument, not %s",
		     idterm->toString().c_str() );
	    return YCPNull();
	}

	YCPValue id = idterm->value(0);

	if ( findWidgetWithId( id,
			       false ) ) // Don't throw exception if not found
	{
            // Already have a widget with that ID?
	    ycperror( "Widget id %s is not unique", id->toString().c_str() );
	    return YCPNull();
	}

	*argnr = 1;
	return id;
    }
    else
    {
	*argnr = 0;
	return YCPVoid();	// no `id() specified -> use "nil"
    }
}


YCPList
YCPDialogParser::getWidgetOptions( const YCPTerm & term, int *argnr )
{
    if ( term->size() > *argnr
	 && term->value( *argnr )->isTerm()
	 && term->value( *argnr )->asTerm()->name() == YUISymbol_opt )
    {
	YCPTerm optterm = term->value( *argnr )->asTerm();
	*argnr = *argnr + 1;
	return optterm->args();
    }
    else return YCPList();
}


void
YCPDialogParser::logUnknownOption( const YCPTerm & term, const YCPValue & option )
{
    ycperror( "Unknown option %s in %s widget",
	      option->toString().c_str(), term->name().c_str() );
}


void
YCPDialogParser::rejectAllOptions( const YCPTerm & term, const YCPList & optList )
{
    for ( int o=0; o < optList->size(); o++ )
    {
	logUnknownOption( term, optList->value(o) );
    }
}



// EOF

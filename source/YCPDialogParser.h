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

  File:		YCPDialogParser.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YCPDialogParser_h
#define YCPDialogParser_h

#include <vector>
#include <ycp/YCPTerm.h>
#include <ycp/YCPList.h>

#include <yui/YTypes.h>

using std::vector;

class YWidget;
class YMenu;
class YMenuButton;
class YTableHeader;
class YWidgetOpt;


/**
 * Parser for YCP dialog terms.
 **/
class YCPDialogParser
{
public:

    /**
     * Parse a widget tree term.
     *
     * 'parent': the widget or dialog this widget is contained in
     * 'term': YCPTerm describing the widget
     *
     * Return value: pointer to the new widget or 0 if it was not successful.
     * And error has been logged in this case
     **/
    static YWidget * parseWidgetTreeTerm( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term );

    /**
     * Overloaded version for convenience. Supplies empty widget options.
     **/
    static YWidget * parseWidgetTreeTerm( YWidget *parent, const YCPTerm & term );

    /**
     * Find a widget in the widget tree below 'widgetRoot' with the specified ID.
     * Throws exceptions if 'doThrow' is 'true' and there is no widget with that ID.
     **/
    static YWidget * findWidgetWithId( YWidget *widgetRoot, const YCPValue & id, bool doThrow = true );

    /**
     * Find a widget in the current (topmost) dialog with the specified ID.
     * Throws exceptions if 'doThrow' is 'true' and there is no widget with that ID.
     **/
    static YWidget * findWidgetWithId( const YCPValue & id, bool doThrow = true );

    /**
     * Checks if the given value is either a symbol or a term `id().
     *
     * Return value: 'true' if 'val' is a symbol or a valid `id().
     **/
    static bool isSymbolOrId( const YCPValue & val );

    /**
     * Checks if the given value is a term with the symbol 'id and
     * size one. Logs an error if this is not so and 'complain' is set.
     *
     * Return value: 'true' if 'val' is a valid `id().
     **/
    static bool checkId( const YCPValue & val, bool complain = true );

    /**
     * Parse an ID term like `id(`some_id) or `id("some_id") or `id( YCPValue ).
     * Return the content of that ID term (`some_id, "some_id", or the YCPValue).
     **/
    static YCPValue parseIdTerm( const YCPValue & v );

    /**
     * Parse a list of numeric values and return it as a vector<float>.
     * Throws exceptions upon syntax errors.
     **/
    static vector<float> parseNumList( const YCPList & yList );


protected:

    static YWidget * parseReplacePoint( YWidget *parent, YWidgetOpt & opt,
					const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseEmpty( YWidget *parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr );

    /**
     * HSpacing, VSpacing, HStretch, VStretch
     **/
    static YWidget * parseSpacing( YWidget *parent, YWidgetOpt & opt,
				   const YCPTerm & term, const YCPList & optList, int argnr,
				   YUIDimension dim, bool stretchable );

    static YWidget * parseFrame( YWidget *parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseCheckBoxFrame( YWidget *parent, YWidgetOpt & opt,
					 const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseWeight( YWidget *parent, YWidgetOpt & opt,
				  const YCPTerm & term, const YCPList & optList, int argnr,
				  YUIDimension dim );

    /**
     * Alignment: Left, Right, Top, Bottom, HCenter, VCenter, HVCenter
     **/
    static YWidget * parseAlignment( YWidget *parent, YWidgetOpt & opt,
				     const YCPTerm & term, const YCPList & optList, int argnr,
				     YAlignmentType horAlign, YAlignmentType vertAlign );

    static YWidget * parseMarginBox( YWidget * parent, YWidgetOpt & opt,
				     const YCPTerm & term, const YCPList & optList, int argnr );

    /**
     * MinWidth, MinHeight, MinSize
     **/
    static YWidget * parseMinSize( YWidget * parent, YWidgetOpt & opt,
				   const YCPTerm & term, const YCPList & optList, int argnr,
				   bool hor, bool vert );

    /**
     * HSquash, VSquash, HVSquash
     **/
    static YWidget * parseSquash( YWidget *parent, YWidgetOpt & opt,
				  const YCPTerm & term, const YCPList & optList, int argnr,
				  bool hsquash, bool vsquash );

    /**
     * LayoutBox: HBox, VBox
     **/
    static YWidget * parseLayoutBox( YWidget *parent, YWidgetOpt & opt,
				     const YCPTerm & term, const YCPList & optList, int argnr,
				     YUIDimension dim );
    /**
     * ButtonBox
     **/ 
    static YWidget * parseButtonBox( YWidget *parent, YWidgetOpt & opt,
				      const YCPTerm & term, const YCPList & optList, int argnr );

    /**
     * Label, Heading
     **/
    static YWidget * parseLabel( YWidget *parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr,
				 bool heading );

    static YWidget * parseDateField( YWidget *parent, YWidgetOpt & opt,
				     const YCPTerm & term, const YCPList & optList, int argnr);

    static YWidget * parseTimeField( YWidget *parent, YWidgetOpt & opt,
				     const YCPTerm & term, const YCPList & optList, int argnr);

    static YWidget * parseRichText( YWidget *parent, YWidgetOpt & opt,
				    const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseLogView( YWidget *parent, YWidgetOpt & opt,
				   const YCPTerm & term, const YCPList & optList, int argnr );

    /**
     * PushButton, IconButton
     **/
    static YWidget * parsePushButton( YWidget *parent, YWidgetOpt & opt,
				      const YCPTerm & term, const YCPList & optList, int argnr,
				      bool isIconButton );

    static YWidget * parseMenuButton( YWidget *parent, YWidgetOpt & opt,
				      const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseCheckBox( YWidget *parent, YWidgetOpt & opt,
				    const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseRadioButton( YWidget *parent, YWidgetOpt & opt,
				       const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseRadioButtonGroup( YWidget *parent, YWidgetOpt & opt,
					    const YCPTerm & term, const YCPList & optList, int argnr );

    /**
     * InputField, Password
     **/
    static YWidget * parseInputField( YWidget *parent, YWidgetOpt & opt,
				     const YCPTerm & term, const YCPList & optList, int argnr,
				     bool passwordMode, bool bugCompatibilityMode = false );

    static YWidget * parseMultiLineEdit( YWidget *parent, YWidgetOpt & opt,
					 const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseSelectionBox( YWidget *parent, YWidgetOpt & opt,
					const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseMultiSelectionBox( YWidget *parent, YWidgetOpt & opt,
					     const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseComboBox( YWidget *parent, YWidgetOpt & opt,
				    const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseTree( YWidget *parent, YWidgetOpt & opt,
				const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseTable( YWidget *parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr );

    static YTableHeader * parseTableHeader( const YCPTerm & headerTerm );

    static YWidget * parseProgressBar( YWidget *parent, YWidgetOpt & opt,
				       const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseImage( YWidget *parent, YWidgetOpt & opt,
				 const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseIntField( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
				    const YCPList & optList, int argnr );

    static YWidget * parsePackageSelector( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
					   const YCPList & optList, int argnr );

    static YWidget * parsePkgSpecial( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
				      const YCPList & optList, int argnr );

    static YWidget * parseWizard( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
				  const YCPList & optList, int argnr );

    static YWidget * parseDummySpecialWidget( YWidget *parent, YWidgetOpt & opt,
					      const YCPTerm & term, const YCPList & optList, int argnr );

    static YWidget * parseDownloadProgress( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
					    const YCPList & optList, int argnr );

    static YWidget * parseBarGraph( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
				    const YCPList & optList, int argnr );

    static YWidget * parseDumbTab( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
				   const YCPList & optList, int argnr );

    static YWidget * parseMultiProgressMeter( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
					      const YCPList & optList, int argnr,
					      YUIDimension dim );

    static YWidget * parseSlider( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
				  const YCPList & optList, int argnr );

    static YWidget * parsePartitionSplitter( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
					     const YCPList & optList, int argnr );

    static YWidget * parsePatternSelector( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
					   const YCPList & optList, int argnr );

    static YWidget * parseSimplePatchSelector( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
					       const YCPList & optList, int argnr );

    static YWidget * parseTimezoneSelector ( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
					     const YCPList & optList, int argnr );

    static YWidget * parseGraph ( YWidget *parent, YWidgetOpt & opt, const YCPTerm & term,
				  const YCPList & optList, int argnr );

    static YWidget * parseBusyIndicator( YWidget *parent, YWidgetOpt & opt,
				       const YCPTerm & term, const YCPList & optList, int argnr );
    /**
     * Look for a widget id in a widget term. If it finds one, returns
     * it and sets argnr to point after `id(), whether it turned out valid
     * or not. Otherwise it creates a new unique widget id and sets 
     * argnr to 0. For example PushButton( `id( 17 ), .... ) has with id 17.
     * If it cannot find an id, or the id is a duplicate of an existing one
     * YCPNull is returned 
     * Return value: The widget id on success or YCPNull on failure
     **/
    static YCPValue getWidgetId( const YCPTerm & term, int *argnr );

    /**
     * Look for widget options in the term. Returns the list
     * of options if available, otherwise an empty list.
     * Increases argnr by 1 if options are found.
     * For example PushButton( `id( 17 ), `opt( `kilroy, `color( `red ) )
     * has the option list [ `kilroy, `color( `red ) ]
     * 'term': the Widget term
     * 'argnr' (in/out): The number of the first non-id argument.
     * Returns the number of the first non-id and non-opt argument.
     *
     * Return value: The option list, which may be empty, but never YCPNull
     **/
    static YCPList getWidgetOptions( const YCPTerm & term, int *argnr );

    /**
     * Logs a warning for an unknown widget option
     * 'term:  the widget term, e.g. PushButton( `opt( `unknown ), ... )
     * 'option': the unknown option itself
     **/
    static void logUnknownOption( const YCPTerm & term, const YCPValue & option );

    /**
     * Logs warning messages for all widget options other than the
     * standard ones - for widgets that don't handle any options.
     * 'term': the widget term, e.g. PushButton( `opt( `unknown ), ... )
     * 'optList': the list of options not yet processed
     **/
    static void rejectAllOptions( const YCPTerm & term, const YCPList & optList );
    
    /**
     * Return 'true' if 'str' starts with 'word'. This is case insensitive.
     **/
    static bool startsWith( const string & str, const char * word );


private:

    /**
     * Constructor.
     *
     * Private since all functions of this class are static.
     **/
    YCPDialogParser() {}

};


#endif // YCPDialogParser_h

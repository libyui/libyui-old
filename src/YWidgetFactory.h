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

  File:		YWidgetFactory.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YWidgetFactory_h
#define YWidgetFactory_h

#include <string>

#include "YTypes.h"

using std::string;

class YWidget;
class YAlignment;
class YButtonBox;
class YCheckBox;
class YCheckBoxFrame;
class YComboBox;
class YDialog;
class YEmpty;
class YFrame;
class YImage;
class YInputField;
class YIntField;
class YLabel;
class YLayoutBox;
class YLogView;
class YMenuButton;
class YMultiLineEdit;
class YMultiSelectionBox;
class YPackageSelector;
class YProgressBar;
class YPushButton;
class YRadioButton;
class YRadioButtonGroup;
class YReplacePoint;
class YRichText;
class YSelectionBox;
class YSpacing;
class YSquash;
class YTable;
class YTableHeader;
class YTimeField;
class YTree;
class YBusyIndicator;



/**
 * Abstract widget factory for mandatory widgets.
 * Use YOptionalWidgetFactory for optional ("special") widgets.
 *
 * Refer to the respective widget's documentation (in the header file) for
 * documentation about the function parameters.
 **/
class YWidgetFactory
{
public:

    //
    // Dialogs
    //

    YDialog *			createMainDialog	( YDialogColorMode colorMode = YDialogNormalColor );
    YDialog *			createPopupDialog	( YDialogColorMode colorMode = YDialogNormalColor );
    virtual YDialog *		createDialog		( YDialogType dialogType, YDialogColorMode colorMode = YDialogNormalColor ) = 0;

    //
    // Layout Boxes
    //

    YLayoutBox *		createVBox		( YWidget * parent );
    YLayoutBox *		createHBox		( YWidget * parent );
    virtual YLayoutBox *	createLayoutBox		( YWidget * parent, YUIDimension dimension )				= 0;
    virtual YButtonBox *	createButtonBox		( YWidget * parent )							= 0;

    //
    // Common Leaf Widgets
    //

    virtual YPushButton *	createPushButton	( YWidget * parent, const string & label )				= 0;
    virtual YLabel *		createLabel		( YWidget * parent, const string & text, bool isHeading = false, bool isOutputField = false ) = 0;
    YLabel *			createHeading		( YWidget * parent, const string & label );
    virtual YInputField *	createInputField	( YWidget * parent, const string & label, bool passwordMode = false )	= 0;
    virtual YCheckBox *		createCheckBox		( YWidget * parent, const string & label, bool isChecked = false )	= 0;
    virtual YRadioButton *	createRadioButton	( YWidget * parent, const string & label, bool isChecked = false )	= 0;
    virtual YComboBox *		createComboBox		( YWidget * parent, const string & label, bool editable	 = false )	= 0;
    virtual YSelectionBox * 	createSelectionBox	( YWidget * parent, const string & label )				= 0;
    virtual YTree *		createTree		( YWidget * parent, const string & label )				= 0;
    virtual YTable *		createTable		( YWidget * parent, YTableHeader * header, bool multiSelection = false  ) = 0;
    virtual YProgressBar *	createProgressBar	( YWidget * parent, const string & label, int maxValue = 100 )		= 0;
    virtual YRichText *		createRichText		( YWidget * parent, const string & text = string(), bool plainTextMode = false )	= 0;
    virtual YBusyIndicator *	createBusyIndicator	( YWidget * parent, const string & label, int timeout = 1000 )		= 0;

    //
    // Less Common Leaf Widgets
    //

    YPushButton *		createIconButton	( YWidget * parent, const string & iconName, const string & fallbackTextLabel );
    YLabel *			createOutputField	( YWidget * parent, const string & label );
    virtual YIntField *		createIntField		( YWidget * parent, const string & label, int minVal, int maxVal, int initialVal )	= 0;
    YInputField *		createPasswordField	( YWidget * parent, const string & label );

    virtual YMenuButton *	createMenuButton	( YWidget * parent, const string & label )						= 0;
    virtual YMultiLineEdit * 	createMultiLineEdit	( YWidget * parent, const string & label )						= 0;
    virtual YImage *		createImage		( YWidget * parent, const string & imageFileName, bool animated = false )		= 0;
    virtual YLogView *		createLogView		( YWidget * parent, const string & label, int visibleLines, int storedLines = 0 )	= 0;
    virtual YMultiSelectionBox *createMultiSelectionBox ( YWidget * parent, const string & label )						= 0;

    virtual YPackageSelector *	createPackageSelector	( YWidget * parent, long ModeFlags = 0 )		= 0;
    virtual YWidget *		createPkgSpecial	( YWidget * parent, const string & subwidgetName ) 	= 0; // NCurses only

    //
    // Layout Helpers
    //

    YSpacing *			createHStretch		( YWidget * parent );
    YSpacing *			createVStretch		( YWidget * parent );
    YSpacing *			createHSpacing		( YWidget * parent, YLayoutSize_t size = 1.0 );
    YSpacing *			createVSpacing		( YWidget * parent, YLayoutSize_t size = 1.0 );
    virtual YSpacing *		createSpacing		( YWidget * parent, YUIDimension dim, bool stretchable = false, YLayoutSize_t size = 0.0 ) = 0;
    virtual YEmpty *		createEmpty		( YWidget * parent )	= 0;

    YAlignment *		createLeft		( YWidget * parent );
    YAlignment *		createRight		( YWidget * parent );
    YAlignment *		createTop		( YWidget * parent );
    YAlignment *		createBottom		( YWidget * parent );
    YAlignment *		createHCenter		( YWidget * parent );
    YAlignment *		createVCenter		( YWidget * parent );
    YAlignment *		createHVCenter		( YWidget * parent );

    YAlignment *		createMarginBox		( YWidget * parent, YLayoutSize_t horMargin, YLayoutSize_t vertMargin );
    YAlignment *		createMarginBox		( YWidget * parent,
							  YLayoutSize_t leftMargin, YLayoutSize_t rightMargin,
							  YLayoutSize_t topMargin,  YLayoutSize_t bottomMargin );

    YAlignment *		createMinWidth		( YWidget * parent, YLayoutSize_t minWidth  );
    YAlignment *		createMinHeight		( YWidget * parent, YLayoutSize_t minHeight );
    YAlignment *		createMinSize		( YWidget * parent, YLayoutSize_t minWidth, YLayoutSize_t minHeight );

    virtual YAlignment *	createAlignment		( YWidget * parent, YAlignmentType horAlignment, YAlignmentType vertAlignment )	= 0;

    YSquash *			createHSquash		( YWidget * parent );
    YSquash *			createVSquash		( YWidget * parent );
    YSquash *			createHVSquash		( YWidget * parent );
    virtual YSquash *		createSquash		( YWidget * parent, bool horSquash, bool vertSquash ) = 0;

    //
    // Visual Grouping
    //

    virtual YFrame *		createFrame		( YWidget * parent, const string & label )		 = 0;
    virtual YCheckBoxFrame *	createCheckBoxFrame	( YWidget * parent, const string & label, bool checked ) = 0;

    //
    // Logical Grouping
    //

    virtual YRadioButtonGroup *	createRadioButtonGroup	( YWidget * parent )	= 0;
    virtual YReplacePoint *	createReplacePoint	( YWidget * parent )	= 0;


protected:

    friend class YUI;

    /**
     * Constructor.
     *
     * Use YUI::widgetFactory() to get the singleton for this class.
     **/
    YWidgetFactory();

    /**
     * Destructor.
     **/
    virtual ~YWidgetFactory();

}; // class YWidgetFactory


#endif // YWidgetFactory_h

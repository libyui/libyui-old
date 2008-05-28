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

  File:		YWizard.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YWizard_h
#define YWizard_h

#include "YWidget.h"

class YMacroRecorder;
class YWizardPrivate;
class YPushButton;
class YReplacePoint;

#define YWizardID			"wizard"
#define YWizardContentsReplacePointID	"contents"


enum YWizardMode
{
    YWizardMode_Standard,	// Normal wizard (help panel or nothing)
    YWizardMode_Steps,		// Steps visible in left side panel
    YWizardMode_Tree		// Tree in left side panel
};


/**
 * A wizard is a more complex frame typically used for multi-step workflows:
 *
 *    +------------+------------------------------------------------+
 *    |            |                                                |
 *    |            |                                                |
 *    |            |                                                |
 *    |            |                                                |
 *    |            |                                                |
 *    |            |                                                |
 *    |            |                                                |
 *    |            |                                                |
 *    |  Side bar  |                 Content Area                   |
 *    |            |                (YReplacePoint)                 |
 *    |            |                                                |
 *    |            |                                                |
 *    |            |                                                |
 *    |            |                                                |
 *    |            |                                                |
 *    |            |                                                |
 *    |            |  [Back]           [Abort]              [Next]  |
 *    +------------+------------------------------------------------+
 *
 * The side bar can contain help text, a list of steps that are performed, or
 * an embedded tree (much like the YTree widget).
 *
 * The client application creates the wizard and replaces the widget in the
 * content area for each step.
 *
 * The wizard buttons can theoretically be used to do anything, but good UI
 * design will stick to the model above: [Back], [Abort], [Next].
 *
 * If only two buttons are desired, leave the [Back] button's label emtpy. The
 * remaining two buttons will be rearranged accordingly in the button area.
 *
 * In the last step of a multi-step workflow, the [Next] button's label is
 * customarily replaced with a label that indicates that this is the last
 * step. [Accept] is recommended for that button label: [Finish] (as sometimes
 * used in other environments) by no means clearly indicates that this is the
 * positive ending, the final "do it" button. Worse, translations of that are
 * often downright miserable: To German, [Finish] gets translated as [Beenden]
 * which is the same word as "Quit" (used in menus). This does not at all tell
 * the user that that button really performs the requested action the
 * multi-step wizard is all about.
 **/
class YWizard: public YWidget
{
protected:
    /**
     * Constructor.
     *
     * If only two buttons are desired, leave 'backButtonLabel' empty.
     **/
    YWizard( YWidget *	 	parent,
	     const string & 	backButtonLabel,
	     const string & 	abortButtonLabel,
	     const string & 	nextButtonLabel,
	     YWizardMode 	wizardMode = YWizardMode_Standard );

public:

    /**
     * Destructor.
     **/
    virtual ~YWizard();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YWizard"; }

    
    //
    // Wizard basics
    //

    /**
     * Return the wizard mode (what kind of wizard this is):
     * YWizardMode_Standard, YWizardMode_Steps, YWizardMode_Tree
     **/
    YWizardMode wizardMode() const;
    
    /**
     * Return the wizard buttons or 0 if there is no such button.
     *
     * Derived classes are required to implement this.
     **/
    virtual YPushButton * backButton()  const = 0;
    virtual YPushButton * abortButton() const = 0;
    virtual YPushButton * nextButton()  const = 0;

    /**
     * Return the internal contents ReplacePoint.
     *
     * Derived classes are required to implement this.
     **/
    virtual YReplacePoint * contentsReplacePoint() const = 0;

    /**
     * Protect the wizard's "Next" button against disabling.
     **/
    void protectNextButton( bool protect );

    /**
     * Check if the wizard's "Next" button is currently protected against
     * disabling.
     **/
    bool nextButtonIsProtected() const;

    /**
     * Set the label of one of the wizard buttons (backButton(), abortButton(),
     * nextButton() ) if that button is non-null.
     *
     * The default implementation simply calls button->setLabel( newLabel ).
     **/
    virtual void setButtonLabel( YPushButton * button, const string & newLabel );
    
    /**
     * Set the help text.
     **/
    virtual void setHelpText( const string & helpText ) = 0;

    /**
     * Set the dialog icon. An empty icon name clears the current icon.
     **/
    virtual void setDialogIcon( const string & iconName ) = 0;

    /**
     * Set the dialog heading.
     **/
    virtual void setDialogHeading( const string & headingText ) = 0;

    
    //
    // Steps handling
    //
    
    /**
     * Add a step for the steps panel on the side bar.
     * This only adds the step to the internal list of steps.
     * The display is only updated upon calling updateSteps().
     **/
    virtual void addStep( const string & text, const string & id ) = 0;

    /**
     * Add a step heading for the steps panel on the side bar.
     * This only adds the heading to the internal list of steps.
     * The display is only updated upon calling updateSteps().
     **/
    virtual void addStepHeading( const string & text ) = 0;

    /**
     * Delete all steps and step headings from the internal lists.
     * The display is only updated upon calling updateSteps().
     **/
    virtual void deleteSteps() = 0;

    /**
     * Set the current step. This also triggers updateSteps() if necessary.
     **/
    virtual void setCurrentStep( const string & id ) = 0;

    /**
     * Update the steps display: Reflect the internal steps and heading lists
     * in the layout.
     **/
    virtual void updateSteps() = 0;

    
    //
    // Tree handling
    //
    
    /**
     * Add a tree item. If "parentID" is an empty string, it will be a root
     * item. 'text' is the text that will be displayed in the tree, 'id' the ID
     * with which this newly created item can be referenced - and that will be
     * returned when the user clicks on a tree item.
     **/
    virtual void addTreeItem( const string & parentID,
			      const string & text,
			      const string & id	) = 0;

    /**
     * Select the tree item with the specified ID, if such an item exists.
     **/
    virtual void selectTreeItem( const string & id ) = 0;

    /**
     * Returns the current tree selection or an empty string if nothing is
     * selected or there is no tree.
     **/
    virtual string currentTreeSelection() = 0;

    /**
     * Delete all tree items.
     **/
    virtual void deleteTreeItems() = 0;

    
    //
    // Menu handling
    //
    
    /**
     * Add a menu to the menu bar. If the menu bar is not visible yet, it will
     * be made visible. 'text' is the user-visible text for the menu bar
     * (including keyboard shortcuts marked with '&'), 'id' is the menu ID for
     * later addMenuEntry() etc. calls.
     **/
    virtual void addMenu( const string & text,
			  const string & id ) = 0;

    /**
     * Add a submenu to the menu with ID 'parentMenuID'.
     **/
    virtual void addSubMenu( const string & parentMenuID,
			     const string & text,
			     const string & id ) = 0;

    /**
     * Add a menu entry to the menu with ID 'parentMenuID'. 'id' is what will
     * be returned by UI::UserInput() etc. when a user activates this menu entry.
     **/
    virtual void addMenuEntry( const string & parentMenuID,
			       const string & text,
			       const string & id ) = 0;

    /**
     * Add a menu separator to a menu.
     **/
    virtual void addMenuSeparator( const string & parentMenuID ) = 0;

    /**
     * Delete all menus and hide the menu bar.
     **/
    virtual void deleteMenus() = 0;

    /**
     * Show a "Release Notes" button above the "Help" button in the steps panel
     * with the specified label that will return the specified id to
     * UI::UserInput() when clicked.
     **/
    virtual void showReleaseNotesButton( const string & label,
					 const string & id ) = 0;

    //
    // Misc
    //
    
    /**
     * Hide an existing "Release Notes" button.
     **/
    virtual void hideReleaseNotesButton() = 0;

    /**
     * Retranslate internal buttons that are not accessible from the outside:
     * - [Help]
     * - [Steps]
     * - [Tree]
     **/
    virtual void retranslateInternalButtons() = 0;

    /**
     * NOP command to check if a YWizard is running.
     **/
    void ping();


    //
    // Property handling
    //
    
    /**
     * Get a property.
     * Reimplemented from YWidget.
     *
     * This method may throw YUIPropertyExceptions.
     **/
    virtual YPropertyValue getProperty( const string & propertyName );

    /**
     * Return this class's property set.
     * This also initializes the property upon the first call.
     *
     * Reimplemented from YWidget.
     **/
    virtual const YPropertySet & propertySet();


private:

    ImplPtr<YWizardPrivate> priv;
};


#endif // YWizard_h

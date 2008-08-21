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

  File:		YPushButton.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YPushButton_h
#define YPushButton_h

#include "YWidget.h"

class YPushButtonPrivate;

using std::string;


class YPushButton : public YWidget
{
protected:
    /**
     * Constructor.
     **/
    YPushButton( YWidget * parent, const string & label );

public:
    /**
     * Destructor.
     **/
    virtual ~YPushButton();

    /**
     * Return a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YPushButton"; }

    /**
     * Get the label (the text on the button).
     **/
    string label() const;

    /**
     * Set the label (the text on the button).
     *
     * Derived classes are free to reimplement this, but they should call this
     * base class method at the end of the overloaded function.
     **/
    virtual void setLabel( const string & label );

    /**
     * Set this button's icon from an icon file in the UI's default icon
     * directory. Clear the icon if the name is empty.
     *
     * This default implementation does nothing.
     * UIs that can handle icons can choose to overwrite this method.
     **/
    virtual void setIcon( const string & iconName ) {}

    /**
     * Returns 'true' if this is the dialog's default button, i.e. the one
     * button that gets activated if the user hits the [Return] key anywhere in
     * the dialog.
     **/
    bool isDefaultButton() const;

    /**
     * Make this button the default button.
     *
     * Derived classes should reimplement this, but call this base class
     * function in the overwritten function.
     **/
    virtual void setDefaultButton( bool def = true );

    /**
     * Set a predefined role for this button.
     *
     * This is important when the button is a child of a YButtonBox so the
     * layout can be arranged according to the conventions of the current UI or
     * desktop environment.
     *
     * See YButtonBox.h for more details. YButtonRole is defined in YTypes.h
     *
     * The default is YCustomButton, i.e., no predefined role.
     * setFunctionKey() uses some heuristics to map function keys to buttons:
     *
     *     F10 -> YOkButton
     *     F9  -> YCancelButton
     *     F1  -> YHelpButton
     *
     * Derived classes are free to reimplement this, but they should call this
     * base class function in the overwritten function.
     **/
    virtual void setRole( YButtonRole role );

    /**
     * Return the role of this button.
     **/
    YButtonRole role() const;

    /**
     * Assign a function key to this widget
     * (1 for F1, 2 for F2, etc.; 0 for none)
     *
     * Reimplemented from YWidget to map function keys to button roles.
     *
     * Derived classes may want to overwrite this function, but they should
     * call this base class function in the new function.
     **/
    virtual void setFunctionKey( int fkey_no );

    
    /**
     * Returns 'true' if this is a "Help" button.
     *
     * When activated, a help button will traverse up its widget hierarchy and
     * search for the topmost widget with a helpText() set and display that
     * help text in a pop-up dialog (with a local event loop).
     *
     * NOTE that this is only done during YDialog::waitForEvent() (i.e. in YCP
     * UI::WaitForEvent(), UI::UserInput(), UI::TimeoutUserInput() ) and not
     * during YDialog::pollEvent() (i.e. YCP UI::PollInput()) since displaying
     * the help text will block the application until the user closes the help
     * text.
     **/
    bool isHelpButton() const;

    /**
     * Make this button a help button.
     *
     * Derived classes are free to reimplement this, but they should call this
     * base class method in the overloaded function.
     **/
    virtual void setHelpButton( bool helpButton = true );

    /**
     * Set a property.
     * Reimplemented from YWidget.
     *
     * This function may throw YUIPropertyExceptions.
     *
     * This function returns 'true' if the value was successfully set and
     * 'false' if that value requires special handling (not in error cases:
     * those are covered by exceptions).
     **/
    virtual bool setProperty( const string & propertyName,
			      const YPropertyValue & val );

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

    /**
     * Get the string of this widget that holds the keyboard shortcut.
     *
     * Reimplemented from YWidget.
     **/
    virtual string shortcutString() const { return label(); }

    /**
     * Set the string of this widget that holds the keyboard shortcut.
     *
     * Reimplemented from YWidget.
     **/
    virtual void setShortcutString( const string & str )
	{ setLabel( str ); }


private:

    ImplPtr<YPushButtonPrivate> priv;
};


std::ostream & operator<<( std::ostream & stream, YButtonRole role );


typedef YPushButton YIconButton;


#endif // YPushButton_h

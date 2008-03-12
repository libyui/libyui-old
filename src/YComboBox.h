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

  File:		YComboBox.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YComboBox_h
#define YComboBox_h

#include "YSelectionWidget.h"

class YComboBoxPrivate;


/**
 * ComboBox (a.k.a. "drop down box", "drop down selection"):
 *
 * A widget with a drop-down list of predefined values to select from.
 * Optionally, this widget can be created in "editable" mode which means that
 * the user can freely enter any text.
 *
 * In non-editable mode, a ComboBox works very much like a SelectionBox that
 * uses fewer screen space. In that mode, it is recommended to use
 * selectedItem() to retrieve its current value and selectItem() to set it.
 *
 * In editable mode, a ComboBox is more like an InputField with a list to pick
 * predefined values from (for less typing). In that mode, it is recommended to
 * use value() and setValue().
 *
 * In either mode, it might be dangerous to use the iterators the
 * (itemsBegin(), itemsEnd()) the base class (YSelectionWidget) provides to
 * find the currently selected item: The items' "selected" flag may or may not
 * be up to date. YComboBox::selectedItem() makes sure they are up to date.
 **/
class YComboBox : public YSelectionWidget
{
protected:
    /**
     * Constructor.
     *
     * 'editable' means the user can freely enter any value without being
     * restricted to the items of the ComboBox's list.
     **/
    YComboBox( YWidget * parent, const string & label, bool editable );

public:
    /**
     * Destructor.
     **/
    virtual ~YComboBox();

    /**
     * Returns a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YComboBox"; }

    /**
     * Return 'true' if this ComboBox is editable, i.e. if the user can freely
     * enter any value without being restricted to the items of the ComboBox's
     * list.
     *
     * Notice that this can only be set in the constructor.
     **/
    bool editable() const;

    /**
     * Return the value of this ComboBox:
     *
     * The text of a list item if the user (or the appplication) selected a
     * list item or the content of the ComboBox's input field if the ComboBox
     * is editable and the user (or the application) entered text there.
     *
     * See also YComboBox::selectedItem().
     **/
    string value();

    /**
     * Set the value of this ComboBox by string: Try to find a list item with
     * that label and select it.
     *
     * If there is no matching list item, editable ComboBoxes will set their
     * input field to that text. Non-editable ComboBoxes will throw an
     * exception.
     *
     * See also selectItem().
     **/
    void setValue( const string & newText );

    /**
     * Return the (first) selected item or 0 if none is selected or if this
     * ComboBox is editable and the user entered something that does not match
     * any of the ComboBox's list items (in that case, use value() instead).
     *
     * Reimplemented from YSelectionWidget for better reliability: This will
     * compare an editable ComboBox's user input against the text labels of
     * all items and try to return an item if there is any match.
     **/
    virtual YItem * selectedItem();

    /**
     * Return all selected items.
     *
     * This is not particularly useful for ComboBoxes since there can be no
     * more than one selected item anyway; * better use selectedItem() or
     * value() instead.
     *
     * This function does not transfer ownership of those items to the caller,
     * so don't try to delete them!
     *
     * Reimplemented from YSelectionWidget for better reliability.
     **/
    virtual YItemCollection selectedItems();

    /**
     * Select or deselect an item. See also setValue().
     *
     * Reimplemented from YSelectionWidget.
     **/
    virtual void selectItem( YItem * item, bool selected = true );

    /**
     * Get the valid input characters. No input validation is performed (i.e.,
     * the user can enter anything) if this is empty.
     *
     * This is only meaningful for if the ComboBox is editable.
     **/
    string validChars();

    /**
     * Set the valid input characters. No input validation is performed (i.e.,
     * the user can enter anything) if this is empty.
     *
     * This is only meaningful for if the ComboBox is editable.
     *
     * Derived classes are free to reimplement this, but they should call this
     * base class method at the end of the overloaded function.
     **/
    virtual void setValidChars( const string & validChars );

    /**
     * The maximum input length, i.e., the maximum number of characters the
     * user can enter. -1 means no limit.
     *
     * This is only meaningful for if the ComboBox is editable.
     **/
    int inputMaxLength() const;

    /**
     * Set the maximum input length, i.e., the maximum number of characters the
     * user can enter. -1 means no limit.
     *
     * This is only meaningful for if the ComboBox is editable.
     *
     * Derived classes are free to reimplement this, but they should call this
     * base class method at the end of the overloaded function.
     **/
    virtual void setInputMaxLength( int numberOfChars );

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
     * The name of the widget property that will return user input.
     * Inherited from YWidget.
     **/
    const char * userInputProperty() { return YUIProperty_Value; }


protected:

    /**
     * Return this ComboBox's current value as text.
     *
     * Called internally from value(), selectedItem() and related.
     *
     * Derived classes are required to implement this function.
     **/
    virtual string text() = 0;

    /**
     *
     * Set this ComboBox's current value as text.
     *
     * Called internally whenever the content is to change
     * programmatically. Don't call setValue() or selectItem() from here.
     *
     * Derived classes are required to implement this function.
     **/
    virtual void setText( const string & newText ) = 0;


private:

    ImplPtr<YComboBoxPrivate> priv;
};


#endif // YComboBox_h

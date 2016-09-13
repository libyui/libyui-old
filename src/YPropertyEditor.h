
#include <string>

#include <YDialog.h>
#include <YWidget.h>

#include <YDialog.h>
#include <YPushButton.h>
#include <YComboBox.h>
#include <YInputField.h>
#include <YIntField.h>


/**
 * An internal helper class for displaying the widget property editor
 * in the spy dialog.
 */
class YPropertyEditor
{
public:
    /**
     * Constructor
     * @param  widget the target widget
     */
    YPropertyEditor(YWidget * widget);

    /**
     * Display a popup for editing a widget property.
     * @param  property name of the property to edit
     * @return true if the property has been changed, false otherwise
     */
    bool edit(const std::string &property);

private:

    YWidget * _widget;
    YPropertyValue orig;

    /**
     * Is the property read-only?
     * @param  property property name
     * @return true if it is read-only, false if it can be changed
     */
    bool isReadOnly(const std::string &property);

    // UI widgets
    // the main popup
    YDialog *popup;

    // input widgets
    YComboBox *combo;
    YIntField *intfield;
    YInputField *input;

    // buttons
    YPushButton *okButton;
    YPushButton *cancelButton;

    // dialog handlers
    // show the dialog on the screen
    void show(const std::string &property);
    // run the main event loop
    void run(const std::string &property);
    // close the dialog
    void close();
    // is the property editable?
    bool editable(const std::string &property);
};

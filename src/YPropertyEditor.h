
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


    /**
     * Is the property read-only?
     * @param  property Name of the property
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

    /**
     * show the dialog on the screen
     * @param property Name of the property to edit
     */
    void show(const std::string &property);

    /**
     * Run the main event loop
     * @param  property Name of the property to edit
     * @return true if the value has been changed, false otherwise
     */

    bool run(const std::string &property);
    /**
     * Close the dialog window
     */

    void close();
    /**
     * Is the property editable? Editable property is not read-only and
     * it is String, Integer or Boolean type.
     * @param  property Name of the property
     * @return true if the property can be changed
     */
    bool editable(const std::string &property);
};

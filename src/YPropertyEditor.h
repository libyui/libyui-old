
#include <string>

#include <YDialog.h>
#include <YWidget.h>

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
     * @param  property property name
     * @return true if it is read-only, false if it can be changed
     */
    bool isReadOnly(const std::string &property);
};

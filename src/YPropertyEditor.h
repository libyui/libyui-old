
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
     * @return          true if the property has been changed
     */
    bool edit(const std::string &property);
    
private:
    // TODO: use Impl.h ?

    YWidget * _widget;
    bool isReadOnly(const std::string &property);
};


#ifndef YPropertyEditor_h
#define YPropertyEditor_h

#include <string>

class YWidget;
class YPropertyEditorPriv;

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

    virtual ~YPropertyEditor();

    /**
     * Display a popup for editing a widget property.
     * @param  property name of the property to edit
     * @return true if the property has been changed, false otherwise
     */
    bool edit(const std::string &property);

private:

    YPropertyEditorPriv *priv;
};

#endif

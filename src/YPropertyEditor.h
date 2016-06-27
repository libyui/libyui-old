
#include <string>

#include <YDialog.h>
#include <YWidget.h>

class YPropertyEditor
{
public:
    // 
    YPropertyEditor(YDialog *dialog, YWidget * widget);

    // display a popup for editing a property
    void edit(const std::string &property);
    
private:
    // TODO: use Impl.h

    YDialog *_dialog;
    YWidget * _widget;
    
    bool isReadOnly(const std::string &property);
};


#include <string>

#include <YDialog.h>
#include <YWidget.h>

class YPropertyEditor
{
public:
    // 
    YPropertyEditor(YWidget * widget);

    //! display a popup for editing a property
    //! \return true if the property has been changed
    bool edit(const std::string &property);
    
private:
    // TODO: use Impl.h

    YWidget * _widget;
    bool isReadOnly(const std::string &property);
};

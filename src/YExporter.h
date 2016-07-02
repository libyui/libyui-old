
#include <iostream>

#include <YWidget.h>

class YExporter
{
public:

    YExporter(YWidget *w) : _widget(w) {}
    virtual ~YExporter() {}
    
    virtual void serialize(std::ostream &str) = 0;

protected:

    YWidget * _widget;
};

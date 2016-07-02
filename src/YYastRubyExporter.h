
#include <YExporter.h>

class YYastRubyExporter : public YExporter
{
public:

    YYastRubyExporter(YWidget *w) : YExporter(w) {}
    virtual ~YYastRubyExporter() {}
    
    virtual void serialize(std::ostream &str);
    
private:
    
    void serializeWidgetTree(YWidget *w, std::ostream &str, int indentationLevel );
    void serializeWidget( YWidget *w, std::ostream &str, int indentationLevel );
};

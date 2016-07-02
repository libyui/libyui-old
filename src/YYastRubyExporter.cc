
#include <cstring>

#include <YYastRubyExporter.h>
#include <YWidgetID.h>

#define YUILogComponent "ui-yrb-exporter"
#include "YUILog.h"

#define INDENTSIZE 2

static std::string widgetOptions(YWidget *w);
static std::string widgetID(YWidget *w);

static std::string indentation(int indentationLevel)
{
    return std::string( indentationLevel * INDENTSIZE, ' ' );
}

void YYastRubyExporter::serialize(std::ostream &str)
{
    serializeWidgetTree(_widget, str, 0);
}

void YYastRubyExporter::serializeWidgetTree(YWidget *w, std::ostream &ostr, int indentationLevel )
{
    serializeWidget( w, ostr, indentationLevel );

    for ( YWidgetListConstIterator it = w->childrenBegin();
	  it != w->childrenEnd();
	  ++it )
    {
    	YWidget * child = *it;



    	if ( child->hasChildren() )
        {
    	    serializeWidgetTree(child, ostr, indentationLevel + 1 );
        }
    	else
    	    serializeWidget( child, ostr, indentationLevel + 1 );

        if (child != w->lastChild())
            ostr << ",";

        ostr << std::endl;
    }
}


void YYastRubyExporter::serializeWidget( YWidget *w, std::ostream &str, int indentationLevel )
{
    const char * wclass = w->widgetClass();
    if (!wclass) return;

    if (strcmp(wclass, "YQWizardButton") == 0) return;

    const char *targetWidget;

    if (strncmp(wclass, "YAlignment_", sizeof("YAlignment_") - 1) == 0)
        targetWidget = wclass + sizeof("YAlignment_") - 1;
    else if (wclass[0] == 'Y')
        targetWidget = wclass + 1;
    else
        targetWidget = wclass;

    // indentation
    str << indentation( indentationLevel );
    // widget type
    str << targetWidget << "(";

    std::string data;

    if (w->hasId()) data = "Id(" + widgetID(w) + ")";

    std::string opts = widgetOptions(w);

    if (!opts.empty())
    {
        if (!data.empty()) data += ", ";

        data += "Opt(" + opts + ")";
    }


    if (w->propertySet().contains("Label"))
    {
        auto label = w->getProperty("Label").stringVal();
        if (!label.empty())
        {
            if (!data.empty()) data += ", ";

            data += '"' + label + '"';
        }
    }

    if (!data.empty() && w->hasChildren()) data += ',';

    str << data << std::endl;
}

std::string widgetID(YWidget *w)
{
    std::string id(w->id()->toString());

    // convert YCP symbol to Ruby symbol
    if (id.size() > 0 && id[0] == '`')
        id[0] = ':';
    else
    {
        id.insert(0, 1, '"');
        id.append("\"");
    }

    return id;
}

std::string widgetOptions(YWidget *w)
{
    std::string options;

    if (!w->getProperty("Enabled").boolVal())
        options += ":disabled";

    if (w->getProperty("Notify").boolVal())
    {
        if (!options.empty()) options += ", ";
        options += ":notify";
    }

    if (w->getProperty("VStretch").boolVal())
    {
        if (!options.empty()) options += ", ";
        options += ":vstretch";
    }

    if (w->getProperty("HStretch").boolVal())
    {
        if (!options.empty()) options += ", ";
        options += ":hstretch";
    }

    return options;
}
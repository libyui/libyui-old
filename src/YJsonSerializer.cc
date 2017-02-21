
#include <json/json.h>

#include "YDialog.h"
#include "YCheckBox.h"
#include "YInputField.h"
#include "YIntField.h"
#include "YRadioButton.h"
#include "YSpacing.h"
#include "YTable.h"
#include "YWidget.h"
#include "YWidgetID.h"
#include "YWizard.h"

#include "YJsonSerializer.h"

static void serialize_widget_properties(YWidget *widget, Json::Value &json);
static void serialize_widget_data(YWidget *widget, Json::Value &json);
static void serialize_widget_specific_data(YWidget *widget, Json::Value &json);

Json::Value serialize_rec(YWidget *w, bool recursive = true) {
    Json::Value ret;

    serialize_widget_properties(w, ret);
    serialize_widget_data(w, ret);
    serialize_widget_specific_data(w, ret);

    if (recursive && w->hasChildren()) {
        Json::Value widgets;

        for ( YWidgetListConstIterator it = w->childrenBegin(); it != w->childrenEnd(); ++it )
        {
            if (*it)
            {
                Json::Value widget = serialize_rec(*it);
                widgets.append(widget);
            }
        }

        ret["widgets"] = widgets;
    }

    return ret;
}

void YJsonSerializer::save(const Json::Value &json, std::ostream &output)
{
    // use a custom indentation, the default it too big,
    // the dialogs usually have too many nested widgets
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "  ";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(json, &output);
}

void YJsonSerializer::serialize(YWidget *w, std::ostream &output, bool recursive) {
    if (!w) return;
    Json::Value json = serialize_rec(w, recursive);
    save(json, output);
}

void YJsonSerializer::serialize(const std::vector<YWidget*> &widgets, std::ostream &output, bool recursive) {
    Json::Value array;

    for(YWidget *widget: widgets)
    {
        Json::Value json = serialize_rec(widget, recursive);
        array.append(json);
    }

    save(array, output);
}

void serialize_widget_properties(YWidget *widget, Json::Value &json) {
    auto propSet = widget->propertySet();

    if (propSet.contains("WidgetClass"))
        json["class"] = widget->getProperty("WidgetClass").stringVal();

    if (widget->hasId())
        json["id"] = widget->id()->toString();

    if (propSet.contains("Label"))
        json["label"] = widget->getProperty("Label").stringVal();

    if (propSet.contains("DebugLabel") && !widget->getProperty("DebugLabel").stringVal().empty())
        json["debug_label"] = widget->getProperty("DebugLabel").stringVal();

    // only when false
    if (propSet.contains("Enabled") && !widget->getProperty("Enabled").boolVal())
        json["enabled"] = widget->getProperty("Enabled").boolVal();

    // only if set
    if (propSet.contains("ValidChars") && !widget->getProperty("ValidChars").stringVal().empty())
        json["valid_chars"] = widget->getProperty("ValidChars").stringVal();

    // only if set
    if (propSet.contains("Text") && !widget->getProperty("Text").stringVal().empty())
        json["text"] = widget->getProperty("Text").stringVal();

    // only when true
    if (propSet.contains("Notify") && widget->getProperty("Notify").boolVal())
        json["notify"] = widget->getProperty("Notify").boolVal();

    // only when not empty
    if (propSet.contains("IconPath") && !widget->getProperty("IconPath").stringVal().empty())
        json["icon_path"] = widget->getProperty("IconPath").stringVal();

    // only when set
    if (propSet.contains("InputMaxLength") && widget->getProperty("InputMaxLength").integerVal() >= 0)
        json["input_max_length"] = (Json::Value::Int64)widget->getProperty("InputMaxLength").integerVal();

}

void serialize_widget_data(YWidget *widget, Json::Value &json) {
    // generic data
    if (!widget->isEnabled())
        json["enabled"] = widget->isEnabled();

    if (widget->notify())
        json["notify"] = widget->notify();

    if (widget->hasFunctionKey())
        json["fkey"] = widget->functionKey();

    if (widget->stretchable(YD_HORIZ))
        json["hstretch"] = widget->stretchable(YD_HORIZ);

    if (widget->stretchable(YD_VERT))
        json["vstretch"] = widget->stretchable(YD_VERT);

    if (widget->hasWeight(YD_HORIZ))
        json["hweight"] = widget->weight(YD_HORIZ);

    if (widget->hasWeight(YD_VERT))
        json["vweight"] = widget->weight(YD_VERT);
}

// widget specific data
static void serialize_widget_specific_data(YWidget *widget, Json::Value &json) {

    if (auto ch = dynamic_cast<YCheckBox*>(widget))
    {
        if (ch->value() == YCheckBoxState::YCheckBox_dont_care)
            json["value"] = nullptr;
        else
            json["value"] = ch->isChecked();
    }
    else if (auto inp = dynamic_cast<YInputField*>(widget))
    {
        json["value"] = inp->value();
    }
    else if (auto intf = dynamic_cast<YIntField*>(widget))
    {
        json["value"] = intf->value();
        json["min_value"] = intf->minValue();
        json["max_value"] = intf->maxValue();
    }
    else if (auto rb = dynamic_cast<YRadioButton*>(widget))
    {
        json["value"] = rb->value();
    }
    else if (auto sp = dynamic_cast<YSpacing*>(widget))
    {
        if (sp->dimension() == YD_HORIZ)
            json["value"] = sp->preferredWidth();
        else
            json["value"] = sp->preferredHeight();
    }
    else if (auto dg = dynamic_cast<YDialog*>(widget))
    {
        switch (dg->dialogType())
        {
            case YMainDialog:
                json["type"] = "main";
                break;
            case YPopupDialog:
                json["type"] = "popup";
                break;
            case YWizardDialog:
                json["type"] = "wizard";
                break;
        }
    }
    else if (auto tb = dynamic_cast<YTable*>(widget))
    {
        Json::Value header;
        for ( auto idx = 0; idx < tb->columns(); ++idx )
        {
            header.append(tb->header(idx));
        }
        json["header"] = header;

        Json::Value alignment;
        for ( auto idx = 0; idx < tb->columns(); ++idx )
        {
            std::string alignment_str;
            switch (tb->alignment(idx))
            {
                case YAlignUnchanged:
                    alignment_str = "none";
                    break;
                case YAlignBegin:
                    alignment_str = "left";
                    break;
                case YAlignEnd:
                    alignment_str = "right";
                    break;
                case YAlignCenter:
                    alignment_str = "center";
                    break;
            }
            alignment.append(alignment_str);
        }
        json["alignment"] = alignment;
    }
}


#include <json/json.h>

#include "YDialog.h"
#include "YCheckBox.h"
#include "YInputField.h"
#include "YIntField.h"
#include "YRadioButton.h"
#include "YSpacing.h"
#include "YTable.h"
#include "YTree.h"
#include "YTreeItem.h"
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

namespace {
    void add_opt_string_property(YWidget *w, const std::string &name, Json::Value &json, const std::string &key)
    {
        // only when not empty
        if (w->propertySet().contains(name) && !w->getProperty(name).stringVal().empty())
            json[key] = w->getProperty(name).stringVal();
    }
}

void serialize_widget_properties(YWidget *widget, Json::Value &json) {
    auto propSet = widget->propertySet();

    if (propSet.contains("WidgetClass"))
        json["class"] = widget->getProperty("WidgetClass").stringVal();

    if (widget->hasId())
        json["id"] = widget->id()->toString();

    if (propSet.contains("Label"))
        json["label"] = widget->getProperty("Label").stringVal();

    // only when false
    if (propSet.contains("Enabled") && !widget->getProperty("Enabled").boolVal())
        json["enabled"] = widget->getProperty("Enabled").boolVal();

    // only when true
    if (propSet.contains("Notify") && widget->getProperty("Notify").boolVal())
        json["notify"] = widget->getProperty("Notify").boolVal();

    add_opt_string_property(widget, "DebugLabel", json, "debug_label");
    add_opt_string_property(widget, "Text", json, "text");
    add_opt_string_property(widget, "ValidChars", json, "valid_chars");
    add_opt_string_property(widget, "IconPath", json, "icon_path");
    add_opt_string_property(widget, "HelpText", json, "help_text");

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

namespace
{
    void add_items_rec(Json::Value &jitem, const YItem *yitem)
    {
        if (yitem->selected())
            jitem["selected"] = true;

        // handle YTableItem specifically
        if (auto tabitem = dynamic_cast<const YTableItem*>(yitem))
        {
            Json::Value icons, labels;
            std::for_each(tabitem->cellsBegin(), tabitem->cellsEnd(), [&](const YTableCell *ycell)
            {
                icons.append(ycell->iconName());
                labels.append(ycell->label());
            });
            jitem["icons"] = icons;
            jitem["labels"] = labels;
        }
        // else if (auto treeitem = dynamic_cast<const YTreeItem*>(yitem))
        // {
        //     // TODO handle YTreeItem
        // }
        else
        {
            jitem["label"] = yitem->label();

            if (yitem->hasIconName())
                jitem["icon_name"] = yitem->iconName();
        }

        // this is mainly for the generic widgets like YSelectionBox, YComboBox,...
        if (yitem->hasChildren())
        {
            Json::Value children;

            // recursively add the children
            std::for_each(yitem->childrenBegin(), yitem->childrenEnd(), [&](const YItem *ychild)
            {
                Json::Value child;
                add_items_rec(child, ychild);
                children.append(child);
            });

            jitem["children"] = children;
        }
    }
}
// widget specific data
static void serialize_widget_specific_data(YWidget *widget, Json::Value &json) {

    // check all classes, some widgets might be derived from others
    if (auto ch = dynamic_cast<YCheckBox*>(widget))
    {
        if (ch->value() == YCheckBoxState::YCheckBox_dont_care)
            json["value"] = nullptr;
        else
            json["value"] = ch->isChecked();
    }

    if (auto inp = dynamic_cast<YInputField*>(widget))
    {
        json["value"] = inp->value();
        json["password_mode"] = inp->passwordMode();
    }

    if (auto intf = dynamic_cast<YIntField*>(widget))
    {
        json["value"] = intf->value();
        json["min_value"] = intf->minValue();
        json["max_value"] = intf->maxValue();
    }

    if (auto rb = dynamic_cast<YRadioButton*>(widget))
    {
        json["value"] = rb->value();
    }

    if (auto sp = dynamic_cast<YSpacing*>(widget))
    {
        if (sp->dimension() == YD_HORIZ)
            json["value"] = sp->preferredWidth();
        else
            json["value"] = sp->preferredHeight();
    }

    if (auto dg = dynamic_cast<YDialog*>(widget))
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

    if (auto selection = dynamic_cast<YSelectionWidget*>(widget))
    {
        json["items_count"] = selection->itemsCount();
        json["icon_base_path"] = selection->iconBasePath();

        Json::Value items;
        std::for_each(selection->itemsBegin(), selection->itemsEnd(), [&](const YItem *yitem)
        {
            Json::Value item;
            add_items_rec(item, yitem);
            items.append(item);
        });

        json["items"] = items;
    }

    if (auto tb = dynamic_cast<YTable*>(widget))
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

        json["columns"] = tb->columns();
        json["immediate_mode"] = tb->immediateMode();
        json["keep_sorting"] = tb->keepSorting();
        json["hasMultiSelection"] = tb->hasMultiSelection();
    }
}

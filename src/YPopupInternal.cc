#include <YUI.h>
#include <YWidgetFactory.h>
#include <YDialog.h>
#include <YLayoutBox.h>
#include <YAlignment.h>
#include <YButtonBox.h>
#include <YPushButton.h>
#include <YInputField.h>
#include <YSpacing.h>
#include <YEvent.h>

#define YUILogComponent "ui-popup"
#include "YUILog.h"

#include <YPopupInternal.h>

void YPopupInternal::message(const std::string &label)
{
    auto f = YUI::widgetFactory();

    auto popup = f->createPopupDialog();
    auto mb = f->createMarginBox(popup, 1, 0.1);
    auto vbox = f->createVBox(mb);
    f->createLabel(vbox, label);

    auto bbox = f->createButtonBox( vbox );
    auto okButton = f->createPushButton( bbox, "OK" );

    while ( true )
    {
        auto event = popup->waitForEvent();
        if ( event && (event->widget() == okButton || event->eventType() == YEvent::CancelEvent ))
        {
            break;
        }
    }

    popup->destroy();
}

/**
 * Helper method for adding new input fields
 * @param parent Where to add the widget
 * @param val    The initial value
 */
static void addTextField(YWidget *parent, const std::string &val)
{
    auto new_item = YUI::widgetFactory()->createInputField(parent, "");
    new_item->setProperty("Value", YPropertyValue(val));
    new_item->setProperty("HStretch", YPropertyValue(true));
}

bool YPopupInternal::editStringArray(StringArray &array, const std::string &label )
{
    auto f = YUI::widgetFactory();

    auto popup = f->createPopupDialog();
    auto mb = f->createMarginBox(popup, 1, 0.1);
    auto vbox = f->createVBox(mb);
    f->createHeading(vbox, label);
    auto arrayBox = f->createVBox(vbox);

    // access by reference
    for(auto&& str: array) addTextField(arrayBox, str);

    auto addButton = f->createPushButton( vbox, "Add Item" );

    auto spacing = f->createVSpacing( vbox, 1 );
    spacing->setProperty("VStretch", YPropertyValue(true));

    auto bbox = f->createButtonBox( vbox );
    auto okButton = f->createPushButton( bbox, "OK" );
    auto cancelButton = f->createPushButton( bbox, "Cancel" );

    bool ret;

    while ( true )
    {
        auto event = popup->waitForEvent();

        if (!event) continue;

        if ( event->widget() == cancelButton || event->eventType() == YEvent::CancelEvent )
        {
            ret = false;
            break;
        }
        else if ( event->widget() == okButton )
        {
            array.clear();
            for ( auto it = arrayBox->childrenBegin(); it != arrayBox->childrenEnd(); ++it )
            {
                auto widget = dynamic_cast<YInputField*>(*it);

                if (!widget) continue;

                array.push_back(widget->value());
            }

            ret = true;
            break;
        }
        else if ( event->widget() == addButton )
        {
            addTextField(arrayBox, "");
            popup->recalcLayout();
        }
        else
            yuiWarning() << "Unknown event " << event << std::endl;
    }

    popup->destroy();

    return ret;
}

#include <YUI.h>
#include <YWidgetFactory.h>
#include <YDialog.h>
#include <YLayoutBox.h>
#include <YAlignment.h>
#include <YButtonBox.h>
#include <YPushButton.h>
#include <YEvent.h>

#include <YPopupInternal.h>

// For internal use in libyui itself
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


#include <YPropertyEditor.h>

#include <YUI.h>
#include <YWidgetFactory.h>
#include <YDialog.h>
#include <YLayoutBox.h>
#include <YAlignment.h>
#include <YButtonBox.h>
#include <YPushButton.h>
#include <YEvent.h>
#include <YPopupInternal.h>
#include <YComboBox.h>
#include <YInputField.h>
#include <YIntField.h>

#include <limits>

#define YUILogComponent "ui-property-editor"
#include "YUILog.h"

YPropertyEditor::YPropertyEditor(YWidget * widget)
: _widget(widget)
{}

/**
 * Is the widget property read only?
 * @param  property name of the property
 * @return          true if it is read only
 */
bool YPropertyEditor::isReadOnly(const std::string &property)
{
    // is the property read-only?
    YPropertySet propSet = _widget->propertySet();
    for ( YPropertySet::const_iterator it = propSet.propertiesBegin();
          it != propSet.propertiesEnd();
          ++it )
    {
        YProperty prop = *it;

        if (prop.name() == property)
        {
            return prop.isReadOnly();
        }
    }

    // we cannot edit an unknown property, throw an exception
    YUI_THROW( YUIException( "Unknown property: " + property) );

    // FIXME: never reached, just make the compiler happy (can it be improved?)
    return false;
}

// FIXME: split this too long method
bool YPropertyEditor::edit(const std::string &property)
{
    if (!_widget) return false;

    YPropertyValue prop_value = _widget->getProperty( property );
    yuiMilestone() << "editing property \"" << property << "\" (type: " << prop_value.typeAsStr() << ")";

    // is the property read-only?
    if (isReadOnly(property))
    {
        YPopupInternal::message("Property \"" + property + "\" is read only!");
        return false;
    }

    YPropertyType type = prop_value.type();
    // we cannot edit special properties
    if (type != YBoolProperty && type != YStringProperty && type != YIntegerProperty)
    {
        return false;
    }

    YPropertyValue orig = prop_value;

    YWidgetFactory *f = YUI::widgetFactory();

    YDialog *popup = f->createPopupDialog();
    YLayoutBox *vbox = f->createVBox(popup);

    YComboBox *combo = NULL;
    YIntField *intfield = NULL;
    YInputField *input = NULL;

    if (type == YBoolProperty)
    {
        combo = f->createComboBox( vbox, property);
        combo->setNotify( true );

        YItemCollection items;
        items.push_back( new YItem( "true" ) );
        items.push_back( new YItem( "false" ) );
        combo->addItems( items );
        combo->setValue( prop_value.boolVal() ? "true" : "false" );
    }
    else if (type == YIntegerProperty)
    {
        intfield = f->createIntField(vbox, property,
            // we do not know anything about that property so use the
            // maximum and minimum values for an integer
            std::numeric_limits<int>::min(), std::numeric_limits<int>::max(),
            prop_value.integerVal());
        intfield->setNotify( true );
    }
    else if (type == YStringProperty)
    {
        input = f->createInputField(vbox, property);
        input->setNotify( true );
        input->setValue( prop_value.stringVal() );
    }

    YButtonBox * bbox = f->createButtonBox( vbox );
    YPushButton *okButton = f->createPushButton( bbox, "OK" );
    YPushButton *cancelButton = f->createPushButton( bbox, "Cancel" );

    while ( true )
    {
        YEvent * event = popup->waitForEvent();
        if ( event )
        {
            if (event->widget() == cancelButton || event->eventType() == YEvent::CancelEvent )
            {
                // restore the original value
                if (_widget->getProperty( property ) != orig)
                {
                    _widget->setProperty(property, orig);

                    // TODO: DRY this part
                    auto dialog = _widget->findDialog();
                    if (dialog) dialog->recalcLayout();
                }

                break;
            }
            else if (event->widget() == okButton)
            {
                break;
            }
            else if (event->widget() == combo)
            {
                std::string value = combo->value();
                yuiMilestone() << "Value changed to " << value;
                _widget->setProperty(property, YPropertyValue(value == "true"));

                auto dialog = _widget->findDialog();
                if (dialog) dialog->recalcLayout();
            }
            else if (event->widget() == input)
            {
                std::string value = input->value();
                yuiMilestone() << "Value changed to " << value;

                _widget->setProperty(property, YPropertyValue(value));

                auto dialog = _widget->findDialog();
                if (dialog) dialog->recalcLayout();

            }
            else if (event->widget() == intfield)
            {
                int value = intfield->value();
                yuiMilestone() << "Value changed to " << value;

                _widget->setProperty(property, YPropertyValue(value));

                auto dialog = _widget->findDialog();
                if (dialog) dialog->recalcLayout();
            }
        }
    }

    popup->destroy();

    return _widget->getProperty( property ) != orig;
}


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
 * Helper method - refresh the dialog containing the widget
 * @param widget [description]
 */
static void refreshDialog(YWidget *widget)
{
    auto dialog = widget->findDialog();
    if (dialog) dialog->recalcLayout();
}

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

void YPropertyEditor::show(const std::string &property)
{
    YPropertyValue prop_value = _widget->getProperty( property );
    YPropertyType type = prop_value.type();

    // backup the original property value so it can be restored after
    // clicking the [Cancel] button later
    orig = prop_value;

    auto f = YUI::widgetFactory();

    popup = f->createPopupDialog();
    auto vbox = f->createVBox(popup);

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

    auto bbox = f->createButtonBox( vbox );
    okButton = f->createPushButton( bbox, "OK" );
    cancelButton = f->createPushButton( bbox, "Cancel" );
}

void YPropertyEditor::close()
{
    popup->destroy();

    // nullify the widgets, just to be sure...
    popup = NULL;
    okButton = NULL;
    cancelButton = NULL;
    combo = NULL;
    intfield = NULL;
    input = NULL;
}

void YPropertyEditor::run(const std::string &property)
{
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
                    refreshDialog(_widget);
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
                refreshDialog(_widget);
            }
            else if (event->widget() == input)
            {
                std::string value = input->value();
                yuiMilestone() << "Value changed to " << value;

                _widget->setProperty(property, YPropertyValue(value));
                refreshDialog(_widget);
            }
            else if (event->widget() == intfield)
            {
                int value = intfield->value();
                yuiMilestone() << "Value changed to " << value;

                _widget->setProperty(property, YPropertyValue(value));
                refreshDialog(_widget);
            }
        }
    }
}

bool YPropertyEditor::editable(const std::string &property)
{
    YPropertyValue prop_value = _widget->getProperty(property);

    // is the property read-only?
    if (isReadOnly(property))
    {
        YPopupInternal::message("Property \"" + property + "\" is read only!");
        return false;
    }

    YPropertyType type = prop_value.type();
    // edit special properties cannot be edited
    if (type != YBoolProperty && type != YStringProperty && type != YIntegerProperty)
    {
        return false;
    }

    return true;
}

bool YPropertyEditor::edit(const std::string &property)
{
    if (!_widget || !editable(property)) return false;

    yuiMilestone() << "editing property \"" << property << "\" (type: " <<
        _widget->getProperty(property).typeAsStr() << ")";

    show(property);

    run(property);

    bool changed = _widget->getProperty(property) != orig;

    close();

    return changed;
}

/*
  Copyright (C) 2017 SUSE LLC

  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) version 3.0 of the License. This library
  is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
  License for more details. You should have received a copy of the GNU
  Lesser General Public License along with this library; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
  Floor, Boston, MA 02110-1301 USA
*/

#include "YComboBox.h"
#include "YDialog.h"
#include "YCheckBox.h"
#include "YInputField.h"
#include "YPushButton.h"
#include "YRadioButton.h"
#include "YTable.h"

#include "YHttpWidgetsActionHandler.h"

void YHttpWidgetsActionHandler::body(struct MHD_Connection* connection,
    const char* url, const char* method, const char* upload_data,
    size_t* upload_data_size, std::ostream& body, bool *redraw)
{
    if (YDialog::topmostDialog(false))  {
        WidgetArray widgets;

        if (const char* label = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "label"))
            widgets = YWidgetFinder::by_label(label);
        else if (const char* id = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "id"))
            widgets = YWidgetFinder::by_id(id);
        else if (const char* type = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "type"))
            widgets = YWidgetFinder::by_type(type);
        else {
            widgets = YWidgetFinder::all();
        }

        if (widgets.empty()) {
            body << "{ \"error\" : \"Widget not found\" }" << std::endl;
            _error_code = MHD_HTTP_NOT_FOUND;
        }
        else if (const char* action = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "action"))
        {
            std::string value;
            if (const char* val = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "value"))
                value = val;

            _error_code = do_action(widgets, action, value, body);

            // the action possibly changed something in the UI, signalize redraw needed
            if (redraw && _error_code == MHD_HTTP_OK)
                *redraw = true;
        }
        else {
            body << "{ \"error\" : \"Missing action parameter\" }" << std::endl;
            _error_code = MHD_HTTP_NOT_FOUND;
        }
    }
    else {
        body << "{ \"error\" : \"No dialog is open\" }" << std::endl;
        _error_code = MHD_HTTP_NOT_FOUND;
    }
}

std::string YHttpWidgetsActionHandler::contentEncoding()
{
    return "application/json";
}

int YHttpWidgetsActionHandler::do_action(WidgetArray widgets, const std::string &action, const std::string &value, std::ostream& body) {
    yuiMilestone() << "Starting action: " << action << std::endl;

    // TODO improve this, maybe use better names for the actions...

    // press a button
    if (action == "press_button") {
        return action_handler<YPushButton>(widgets, [] (YPushButton *button) {
            yuiMilestone() << "Pressing button \"" << button->label() << '"' << std::endl;
            button->setKeyboardFocus();
            button->activate();
        } );
    }
    // check a checkbox
    else if (action == "check") {
        return action_handler<YCheckBox>(widgets, [] (YCheckBox *checkbox) {
            if (checkbox->isChecked()) return;
            yuiMilestone() << "Checking \"" << checkbox->label() << '"' << std::endl;
            checkbox->setKeyboardFocus();
            checkbox->setChecked(true);
        } );
    }
    // uncheck a checkbox
    else if (action == "uncheck") {
        return action_handler<YCheckBox>(widgets, [] (YCheckBox *checkbox) {
            if (!checkbox->isChecked()) return;
            yuiMilestone() << "Unchecking \"" << checkbox->label() << '"' << std::endl;
            checkbox->setKeyboardFocus();
            checkbox->setChecked(false);
        } );
    }
    // toggle a checkbox (reverse the state)
    else if (action == "toggle") {
        return action_handler<YCheckBox>(widgets, [] (YCheckBox *checkbox) {
            yuiMilestone() << "Toggling \"" << checkbox->label() << '"' << std::endl;
            checkbox->setKeyboardFocus();
            checkbox->setChecked(!checkbox->isChecked());
        } );
    }
    // enter input field text
    else if (action == "enter_text") {
        return action_handler<YInputField>(widgets, [&] (YInputField *input) {
            yuiMilestone() << "Setting value for InputField \"" << input->label() << '"' << std::endl;
            input->setKeyboardFocus();
            input->setValue(value);
        } );
    }
    else if (action == "switch_radio") {
        return action_handler<YRadioButton>(widgets, [&] (YRadioButton *rb) {
            yuiMilestone() << "Activating RadioButton \"" << rb->label() << '"' << std::endl;
            rb->setKeyboardFocus();
            rb->setValue(true);
        } );
    }
    else if (action == "select_combo") {
        return action_handler<YComboBox>(widgets, [&] (YComboBox *cb) {
            yuiMilestone() << "Activating ComboBox \"" << cb->label() << '"' << std::endl;
            cb->setKeyboardFocus();
            cb->setValue(value);
        } );
    }
    else if (action == "select_table") {
        return action_handler<YTable>(widgets, [&] (YTable *tb) {
	    YItem * item = tb->findItem(value, 0);
	    if (item) {
                yuiMilestone() << "Activating Table \"" << tb->label() << '"' << std::endl;
                tb->setKeyboardFocus();
                tb->selectItem(item);
	    }
	    else {
		yuiWarning() << '"' << value << "\" is not a valid item" << std::endl;
	    }
        } );
    }
    // TODO: more actions
    // else if (action == "enter") {
    // }
    else {
        body << "{ \"error\" : \"Unknown action\" }" << std::endl;
        return MHD_HTTP_NOT_FOUND;
    }

    return MHD_HTTP_OK;
}

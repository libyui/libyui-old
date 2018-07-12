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

#include "YDialog.h"
#include "YJsonSerializer.h"
#include "YHttpWidgetsHandler.h"
#include "YWidgetFinder.h"

#include <microhttpd.h>

void YHttpWidgetsHandler::body(struct MHD_Connection* connection,
    const char* url, const char* method, const char* upload_data,
    size_t* upload_data_size, std::ostream& body, bool *redraw)
{
    if (YDialog::topmostDialog(false))  {
        WidgetArray widgets;

        // TODO: allow filtering by both label and type
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
        else {
            // non recursive dump
            YJsonSerializer::serialize(widgets, body, false);
            _error_code = MHD_HTTP_OK;
        }
    }
    else {
        body << "{ \"error\" : \"No dialog is open\" }" << std::endl;
        _error_code = MHD_HTTP_NOT_FOUND;
    }
}

std::string YHttpWidgetsHandler::contentEncoding()
{
    return "application/json";
}


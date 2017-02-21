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

#include "YHttpRootHandler.h"
#include <microhttpd.h>

void YHttpRootHandler::body(struct MHD_Connection* connection,
    const char* url, const char* method, const char* upload_data,
    size_t* upload_data_size, std::ostream& body)
{
    body <<
"<html>"
"  <head><title>LibYUI Embedded Webserver</title></head>"
"  <body>"
"    <h1>LibYUI Embedded Webserver</h1>"
"    <p>This webserver provides a REST API for the LibYUI application.</p>"
"      <p>It can be used for testing and controlling the application in automated tests.</p>"
"      <br>"
"      <h2>Short Documentation</h2>"
"      <h3>Application</h3>"
"      <p>Request: GET <a href='/application'>/application</a></p>"
"      <h4>Description</h4>"
"      <p>Get the application and UI generic properties like text or graphical mode,"
"      dialog size, screen size and supported UI featues.</p>"
"      <h4>Response</h4>"
"      <p>JSON format</p>"
"      <h4>Examples</h4>"
"      <p>"
"        <pre>  curl http://localhost/application</pre>"
"      </p>"
"      <hr>"
"      <h3>Dialog</h3>"
"      <p>Request: GET <a href='/dialog'>/dialog</a></p>"
"      <h4>Description</h4>"
"      <p>Get the complete dialog structure in the JSON format."
"      The result contains a nested structure exactly following the structure of the current dialog.</p>"
"      <h4>Response</h4>"
"      <p>JSON format</p>"
"      <h4>Examples</h4>"
"      <p>"
"        <pre>  curl http://localhost/dialog</pre>"
"      </p>"
"      <hr>"
"      <h3>Specific Widgets</h3>"
"      <p>Request: GET <a href='/widgets'>/widgets</a></p>"
"      <h4>Description</h4>"
"      <p>Return only the selected widgets (in JSON format). The result is"
"        a flat list (no nested structures).</p>"
"      <h4>Parameters</h4>"
"      <ul>"
"        <li><b>id</b> - widget ID serialized as string, might include special characters like backtick (`)</li>"
"        <li><b>label</b> - widget label as currently displayed (i.e. translated!) </li>"
"        <li><b>type</b> - widget type</li>"
"      </ul>"
"      <h4>Response</h4>"
"      <p>JSON format</p>"
"      <h4>Examples</h4>"
"      <p>"
"        <pre>  curl 'http://localhost/widgets?id=`next'</pre>"
"        <pre>  curl 'http://localhost/widgets?Label=Next'</pre>"
"        <pre>  curl 'http://localhost/widgets?type=YCheckBox'</pre>"
"      </p>"
"    </body>"
"</html>";
}

std::string YHttpRootHandler::contentEncoding()
{
    return "text/html";
}

int YHttpRootHandler::errorCode()
{
    return MHD_HTTP_OK;
}

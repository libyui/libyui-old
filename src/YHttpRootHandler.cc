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
#include "YHttpServer.h"

#include <microhttpd.h>
#include <string>
#include <boost/algorithm/string/replace.hpp>

void YHttpRootHandler::body(struct MHD_Connection* connection,
    const char* url, const char* method, const char* upload_data,
    size_t* upload_data_size, std::ostream& body, bool *redraw)
{
    // TODO: save this to a standalone HTML file, editing HTML in a C string
    // literal is cumbersome, use bin2c or something like that for converting
    // to a C source file...
    std::string help =
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
"        <pre>  curl http://localhost:@port@/application</pre>"
"      </p>"
"      <hr>"
"      <h3>Dump Whole Dialog</h3>"
"      <p>Request: GET <a href='/dialog'>/dialog</a></p>"
"      <h4>Description</h4>"
"      <p>Get the complete dialog structure in the JSON format."
"      The result contains a nested structure exactly following the structure of the current dialog.</p>"
"      <h4>Response</h4>"
"      <p>JSON format</p>"
"      <h4>Examples</h4>"
"      <p>"
"        <pre>  curl http://localhost:@port@/dialog</pre>"
"      </p>"

"      <hr>"
"      <h3>Read Specific Widgets</h3>"
"      <p>Request: GET <a href='/widgets'>/widgets</a></p>"
"      <h4>Description</h4>"
"      <p>Return only the selected widgets (in JSON format). The result is"
"        a flat list (no nested structures).</p>"
"      <h4>Parameters</h4>"
"      <p>Filter widgets:"
"        <ul>"
"          <li><b>id</b> - widget ID serialized as string, might include special characters like backtick (`)</li>"
"          <li><b>label</b> - widget label as currently displayed (i.e. translated!) </li>"
"          <li><b>type</b> - widget type</li>"
"        </ul>"
"      </p>"
"      <h4>Response</h4>"
"      <p>JSON format</p>"
"      <h4>Examples</h4>"
"      <p>"
"        <pre>  curl 'http://localhost:@port@/widgets?id=`next'</pre>"
"        <pre>  curl 'http://localhost:@port@/widgets?label=Next'</pre>"
"        <pre>  curl 'http://localhost:@port@/widgets?type=YCheckBox'</pre>"
"      </p>"

"      <hr>"
"      <h3>Change Widgets, Do an Action</h3>"
"      <p>Request: POST /widgets</p>"
"      <h4>Description</h4>"
"      <p>Do an action with specified widgets.</p>"
"      <h4>Parameters</h4>"
"      <p>Filter the widgets, one of:"
"        <ul>"
"          <li><b>id</b> - widget ID serialized as string, might include special characters like backtick (`)</li>"
"          <li><b>label</b> - widget label as currently displayed (i.e. translated!) </li>"
"          <li><b>type</b> - widget type</li>"
"        </ul>"
" Then specify the action:"
"        <ul>"
"          <li><b>action</b> - action to do</li>"
"          <li><b>value</b> (optional) - new value or a parameter of the action</li>"
"        </ul>"
"      </p>"
"      <h4>Response</h4>"
"      <p>JSON format</p>"
"      <h4>Examples</h4>"
"      <p>"
"        <pre>  # press the `next button\n"
"  curl -X POST 'http://localhost:@port@/widgets?id=`next&action=press'</pre>"
"        <pre>  # set value \"test\" for the InputField with label \"Description\"\n"
"  curl -X POST 'http://localhost:@port@/widgets?label=Description&action=enter&value=test'</pre>"
"      </p>"
"    </body>"
"</html>";

    boost::replace_all(help, "@port@", std::to_string(YHttpServer::port_num()));

    body << help;
}

std::string YHttpRootHandler::contentEncoding()
{
    return "text/html";
}

int YHttpRootHandler::errorCode()
{
    return MHD_HTTP_OK;
}

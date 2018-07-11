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

#ifndef YHttpWidgetsActionHandler_h
#define YHttpWidgetsActionHandler_h

#include "YHttpHandler.h"
#include "YWidgetFinder.h"
#include "YWidget.h"

#include <iostream>
#include <functional>
#include <microhttpd.h>

#define YUILogComponent "http-ui"
#include "YUILog.h"

class YHttpWidgetsActionHandler : public YHttpHandler
{

public:

    YHttpWidgetsActionHandler() {}
    virtual ~YHttpWidgetsActionHandler() {}

protected:

    virtual void body(struct MHD_Connection* connection,
        const char* url, const char* method, const char* upload_data,
        size_t* upload_data_size, std::ostream& body);

    virtual int errorCode() {return _error_code;}

    virtual std::string contentEncoding();

private:

    int _error_code;


    // TODO: move this somewhere else...

    int do_action(WidgetArray widgets, const std::string &action, const std::string &value, std::ostream& body);

    template<typename T>
    int action_handler(WidgetArray widgets, std::function<void (T*)> handler_func) {
        for(YWidget *widget: widgets) {
            if (auto w = dynamic_cast<T*>(widget)) {
                try
                {
                    // allow changing only the enabled widgets, disabled ones
                    // cannot be changed by user from the UI, do not be more powerfull
                    if (handler_func && widget->isEnabled()) handler_func(w);
                }
                // some widgets may throw an exception when setting invalid values
                catch (YUIException)
                {
                    return MHD_HTTP_UNPROCESSABLE_ENTITY;
                }
            }
            else {
                // TODO: demangle the C++ names here ?
                // https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
                yuiError() << "Expected " << typeid(T).name() << " widget, found " << widget->widgetClass()
                     << " (" << typeid(*widget).name() << ')' << std::endl;
                return MHD_HTTP_NOT_FOUND;
            }
        }

        return MHD_HTTP_OK;
    }

};

#endif

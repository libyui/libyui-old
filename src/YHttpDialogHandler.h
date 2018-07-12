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

#ifndef YHttpDialogHandler_h
#define YHttpDialogHandler_h

#include "YHttpHandler.h"

class YHttpDialogHandler : public YHttpHandler
{

public:

    YHttpDialogHandler() {}
    virtual ~YHttpDialogHandler() {}

protected:

    virtual void body(struct MHD_Connection* connection,
        const char* url, const char* method, const char* upload_data,
        size_t* upload_data_size, std::ostream& body, bool *redraw);

    virtual int errorCode() {return _error_code;}

    virtual std::string contentEncoding();

private:

    int _error_code;
};

#endif

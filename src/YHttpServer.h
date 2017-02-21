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

#ifndef YHttpServer_h
#define YHttpServer_h

#include <vector>
#include <string>

#include "YHttpServerSockets.h"
#include "YHttpMount.h"

struct MHD_Daemon;

class YHttpServer
{

public:

    static bool enabled()
    {
        static bool enabled = port_num() != 0;
        return enabled;
    }

    static int port_num();

    YHttpServer();

    ~YHttpServer();


    void start();

    void process_data();

    YHttpServerSockets sockets();

    void mount(const std::string& path, const std::string &method, YHttpHandler *handler);

    int handle(struct MHD_Connection* connection,
        const char* url, const char* method, const char* upload_data,
        size_t* upload_data_size);

private:

    struct MHD_Daemon *server;
    std::vector<YHttpMount> _mounts;
};

#endif

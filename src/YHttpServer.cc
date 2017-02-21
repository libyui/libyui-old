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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <microhttpd.h>

#define YUILogComponent "http-ui"
#include "YUILog.h"
#include "YDialog.h"
#include "YJsonSerializer.h"
#include "YUILog.h"

#include "YHttpServer.h"
#include "YHttpHandler.h"
#include "YHttpDialogHandler.h"
#include "YHttpRootHandler.h"
#include "YHttpAppHandler.h"
#include "YHttpWidgetsHandler.h"
#include "YHttpWidgetsActionHandler.h"

int YHttpServer::port_num()
{
    const char* env_port = getenv("YUI_HTTP_PORT");
    return env_port ? atoi(env_port) : 0;
}

// For security reasons accept the connections only from the localhost
// by default, allow using INADDR_ANY only when explicitly allowed
in_addr_t listen_address()
{
    // remote access enabled
    if ((getenv("YUI_HTTP_REMOTE") && strcmp(getenv("YUI_HTTP_REMOTE"), "1") == 0))
    {
        yuiWarning() << "Warning: Enabling remote access!" << std::endl;
        return INADDR_ANY;
    }

    yuiMilestone() << "Only localhost connections allowed." << std::endl;
    // allow only loop back connection ("http://localhost" only)
    return htonl(INADDR_LOOPBACK);
}

YHttpServer::YHttpServer() : server(nullptr)
{
    mount("/", "GET", new YHttpRootHandler());
    mount("/dialog", "GET", new YHttpDialogHandler());
    mount("/widgets", "GET", new YHttpWidgetsHandler());
    mount("/widgets", "POST", new YHttpWidgetsActionHandler());
    mount("/application", "GET", new YHttpAppHandler());
}

YHttpServer::~YHttpServer()
{
    if (server) {
        yuiMilestone() << "Stopping HTTP server" << std::endl;
        MHD_stop_daemon(server);
    }
}

YHttpServerSockets YHttpServer::sockets()
{
    fd_set rs, ws, es;
    FD_ZERO (&rs);
    FD_ZERO (&ws);
    FD_ZERO (&es);
    int max = 0;

    YHttpServerSockets ret;

    if (MHD_YES != MHD_get_fdset(server, &rs, &ws, &es, &max))
    {
        yuiError() << "Cannot read the FD set!" << std::endl;
        return ret;
    }

    for(int i = 0; i <= max; ++i) {
        if (FD_ISSET(i, &rs)) {
            yuiMilestone() << "Reading FD: " << i << std::endl;
            ret.add_read(i);
        }
        if (FD_ISSET(i, &ws)) {
            yuiMilestone() << "Writing FD: " << i << std::endl;
            ret.add_write(i);
        }
        if (FD_ISSET(i, &es)) {
            yuiMilestone() << "Exception FD: " << i << std::endl;
            ret.add_exception(i);
        }
    }

    if (ret.empty())
        yuiWarning() << "Not watching any FD!" << std::endl;

    return ret;
}

int YHttpServer::handle(struct MHD_Connection* connection,
    const char* url, const char* method, const char* upload_data,
    size_t* upload_data_size)
{
    yuiMilestone() << "Processing " << method << " request: "<< url << ", data size: " << *upload_data_size << std::endl;

    // find the handler
    for(YHttpMount m: _mounts)
    {
        if (m.handles(url, method))
            return m.handler()->handle(connection, url, method, upload_data, upload_data_size);
    }

    // if not found create an empty 404 error response
    yuiMilestone() << "URL path/method not found" << std::endl;
    struct MHD_Response* response = MHD_create_response_from_buffer(0, 0, MHD_RESPMEM_PERSISTENT);
    return MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
}

static int
requestHandler(void *srv,
          struct MHD_Connection *connection,
          const char *url,
          const char *method,
          const char *version,
          const char *upload_data, size_t *upload_data_size, void **ptr)
{
    // remember the callback status
    static int aptr;

    if (&aptr != *ptr)
    {
        // do not respond on first call, it's used for the initial check to close invalid requests early
        *ptr = &aptr;
        // continue processing the request
        return MHD_YES;
    }
    // reset
    *ptr = NULL;

    YHttpServer *server = (YHttpServer *)srv;

    return server->handle(connection, url, method, upload_data, upload_data_size);
}

static int onConnect(void *srv, const struct sockaddr *addr, socklen_t addrlen) {
    if (addr->sa_family == AF_INET) {
        struct sockaddr_in *addr_in = (struct sockaddr_in *) addr;
        // macro INET_ADDRSTRLEN contains the maximum length of an IPv4 address
        // (INET6_ADDRSTRLEN is for IPv6)
        char buffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr_in->sin_addr), buffer, INET_ADDRSTRLEN);
        yuiMilestone() << "Received connection from " << buffer << std::endl;
    }

    return MHD_YES;
}

void YHttpServer::start()
{
    sockaddr_in server_socket;
    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(port_num());
    server_socket.sin_addr.s_addr = listen_address();

    server = MHD_start_daemon (
                        // enable debugging output (on STDERR)
                        MHD_USE_DEBUG,
                        // the port number to use
                        port_num(),
                        // handler for new connections
                        &onConnect, this,
                        // handler for processing requests
                        &requestHandler, this,
                        // enable reusing the socket after quick restart
                        MHD_OPTION_LISTENING_ADDRESS_REUSE, (unsigned int) 1,
                        // set the port and interface to listen to
                        MHD_OPTION_SOCK_ADDR, &server_socket,
                        // finish the argument list
                        MHD_OPTION_END);

    if (server == nullptr) {
      std::cerr << "Cannot start the HTTP server at port " << port_num() << std::endl;
      yuiError() << "Cannot start the HTTP server at port " << port_num() << std::endl;
      // FIXME: exit when failed?
    }
    else {
        yuiWarning() << "Started HTTP server at port " << port_num() << std::endl;
    }
}

void YHttpServer::process_data()
{
    yuiMilestone() << "Processing HTTP server data..." << std::endl;
    MHD_run(server);
}

void YHttpServer::mount(const std::string& path, const std::string &method, YHttpHandler *handler)
{
    _mounts.push_back(YHttpMount(path, method, handler));
}

//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_SOCKET_H
#define WEBSERV_SOCKET_H

#include "../webserv.h"

class Socket {

private:

public:
    Socket();
    ~Socket();

    static int setupClient(int server_socket_fd);
    static int setupServer(int port);

private:
    static void set_non_blocking(int fd);
};


#endif //WEBSERV_SOCKET_H

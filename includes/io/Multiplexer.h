//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_MULTIPLEXER_H
#define WEBSERV_MULTIPLEXER_H

#include "../webserv.h"
#include "../../includes/domain/EventHandler.h"
#include "../../includes/network/Socket.h"

class Multiplexer {

public:
    Multiplexer();

    ~Multiplexer();

    void event_loop(int server_socket_fd);
};


#endif //WEBSERV_MULTIPLEXER_H

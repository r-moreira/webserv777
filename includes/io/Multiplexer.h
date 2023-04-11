//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_MULTIPLEXER_H
#define WEBSERV_MULTIPLEXER_H

#include "../webserv.h"
#include "../../includes/domain/EventHandler.h"
#include "../../includes/network/Socket.h"
#include "../network/Server.h"

class Multiplexer {
private:
    std::vector<Server> _servers;
    int _epoll_fd;
    struct epoll_event _epoll_events[MAX_EPOLL_EVENTS];

public:
    Multiplexer(const std::vector<Server>& servers);

    ~Multiplexer();

    void event_loop();

private:
    bool is_server_fd(int fd);
};


#endif //WEBSERV_MULTIPLEXER_H
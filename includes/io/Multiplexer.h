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

public:
    Multiplexer(const std::vector<Server>& servers);

    ~Multiplexer();

    void event_loop();

private:
    bool is_server_fd(int fd);

    int wait_events(epoll_event *_epoll_events) const;

    int server_event_callback(int client_fd) const;

    int client_read_event_callback(Event *event) const;

    int client_write_event_callback(Event *event) const;
};


#endif //WEBSERV_MULTIPLEXER_H
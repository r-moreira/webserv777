//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_MULTIPLEXER_HPP
#define WEBSERV_MULTIPLEXER_HPP

#include "../utils/Logger.hpp"
#include "../webserv.hpp"
#include "../../includes/domain/EventHandler.hpp"
#include "../../includes/network/Socket.hpp"
#include "../config/Server.hpp"

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

    int server_event_callback(int server_fd, int client_fd) const;

    int client_read_event_callback(Event *event) const;

    int client_write_event_callback(Event *event) const;
};


#endif //WEBSERV_MULTIPLEXER_HPP
//
// Created by rmore on 15/03/2023.
//

#include "../../includes/io/Multiplexer.hpp"

Multiplexer::Multiplexer(const std::vector<Server> &servers) : _servers(servers) {
    this->_epoll_fd = epoll_create1(0);
    if (_epoll_fd == -1) {
        Logger::error("Error while creating epoll fd" + std::string(strerror(errno)));
        exit(EXIT_FAILURE);
    }

    std::vector<Server>::iterator it;
    for (it = _servers.begin(); it != _servers.end(); it++) {

        int server_socket_fd = it->getFd();
        struct epoll_event server_event = {};

        server_event.events = EPOLLIN;
        server_event.data.fd = server_socket_fd;

        if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, server_socket_fd, &server_event) == -1) {
            Logger::error("Error while adding server socket to epoll: " + std::string(strerror(errno)));
            exit(EXIT_FAILURE);
        }
    }
}

Multiplexer::~Multiplexer() {}


void Multiplexer::event_loop() {
    Logger::info("Starting event loop");

    struct epoll_event epoll_events[MAX_EPOLL_EVENTS];

    while (true) {
        int nfds = wait_events(epoll_events);

        for (int i = 0; i < nfds; i++) {
            if (is_server_fd(epoll_events[i].data.fd)) {
                int client_fd = Socket::setupClient(epoll_events[i].data.fd);

                if (client_fd < 0) break;
                if (this->server_event_callback(epoll_events[i].data.fd, client_fd) == -1) continue;

            } else {
                Event *event = (Event *) epoll_events[i].data.ptr;
                EventHandler eventHandler(*event, this->_servers);
                eventHandler.process_event();

                switch (event->getEventStatus()) {
                    case Event::Status::Reading:
                        if (client_read_event_callback(event) == -1) continue;
                        break;
                    case Event::Status::Writing:
                        if (client_write_event_callback(event) == -1) continue;
                        break;
                    case Event::Status::Ended:
                        delete event;
                        break;
                    default:
                        Logger::error("Invalid event_status on event_loop");\
                }

            }
        }
    }
}

bool Multiplexer::is_server_fd(int fd) {
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        if (it->getFd() == fd) return true;
    return false;
}

int Multiplexer::wait_events(epoll_event *epoll_events) const {

    int nfds = epoll_wait(this->_epoll_fd, epoll_events, MAX_EPOLL_EVENTS, -1);
    if (nfds == -1) {
        Logger::error("Error while waiting for epoll events: " + std::string(strerror(errno)));
        return -1;
    }
    return nfds;
}

int Multiplexer::server_event_callback(int server_fd, int client_fd) const {
    struct epoll_event client_event = {};
    Event *event = new Event(server_fd, client_fd);

    client_event.data.ptr = event;
    client_event.events = EPOLLIN;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) == -1) {
        Logger::error("Error while adding client to epoll: " + std::string(strerror(errno)));
        return -1;
    }
    return 1;
}

int Multiplexer::client_read_event_callback(Event *event) const {
    struct epoll_event client_event = {};

    client_event.events = EPOLLIN;
    client_event.data.ptr = event;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, event->getClientFd(), &client_event) < 0) {
        Logger::error("Error while adding reading event to epoll: " + std::string(strerror(errno)));
        return -1;
    }
    return 1;
}

int Multiplexer::client_write_event_callback(Event *event) const {
    struct epoll_event client_event = {};

    client_event.events = EPOLLOUT;
    client_event.data.ptr = event;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, event->getClientFd(), &client_event) < 0) {
        Logger::error("Error while adding writing event to epoll: " + std::string(strerror(errno)));
        return -1;
    }
    return 1;
}

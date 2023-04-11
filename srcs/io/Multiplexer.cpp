//
// Created by rmore on 15/03/2023.
//

#include "../../includes/io/Multiplexer.h"

Multiplexer::Multiplexer(const std::vector<Server> &servers) : _servers(servers) {
    this->_epoll_fd = epoll_create1(0);
    if (_epoll_fd == -1) {
        std::cerr << RED << "Error while creating epoll fd: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<Server>::iterator it;
    for (it = _servers.begin(); it != _servers.end(); it++) {
        int server_socket_fd = it->getFd();

        struct epoll_event server_event;
        server_event.events = EPOLLIN;
        server_event.data.fd = server_socket_fd;

        if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, server_socket_fd, &server_event) == -1) {
            std::cerr << RED << "Error while adding server socket to epoll: " << strerror(errno) << RESET << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

Multiplexer::~Multiplexer() {}

bool Multiplexer::is_server_fd(int fd) {
    for ( std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
        if (it->getFd() == fd) return true;
    return false;
}

void Multiplexer::event_loop() {

    while (true) {
        int nfds = epoll_wait(this->_epoll_fd, _epoll_events, MAX_EPOLL_EVENTS, 3000);
        if (nfds == -1) {
            std::cerr << RED << "Error while waiting for epoll events: " << strerror(errno) << RESET
                      << std::endl;
            continue;
        }

        for (int i = 0; i < nfds; i++) {
            struct epoll_event client_event;

            if (is_server_fd(_epoll_events[i].data.fd)) {

                int client_fd = Socket::setupClient(_epoll_events[i].data.fd);
                if (client_fd < 0) break;

                Event *event = new Event(client_fd);
                client_event.data.ptr = event;
                client_event.events = EPOLLIN;

                if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) == -1) {
                    std::cerr << RED << "Error while adding client to epoll: " << strerror(errno) << RESET
                              << std::endl;
                    continue;
                }
            } else {
                Event *event = (Event *) _epoll_events[i].data.ptr;
                EventHandler eventHandler(*event);
                eventHandler.process_event();

                switch (event->getEventStatus()) {
                    case Reading:
                        client_event.events = EPOLLIN;
                        client_event.data.ptr = event;
                        if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, event->getClientFd(), &client_event) < 0) {
                            std::cerr << RED << "Error while adding reading event to epoll: " << strerror(errno)
                                      << RESET << std::endl;
                            continue;
                        }
                        break;
                    case Writing:
                        client_event.events = EPOLLOUT;
                        client_event.data.ptr = event;
                        if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, event->getClientFd(), &client_event) < 0) {
                            std::cerr << RED << "Error while adding writing event to epoll: " << strerror(errno)
                                      << RESET << std::endl;
                            continue;
                        }
                        break;
                    case Ended:
                        delete event;
                        break;
                    default:
                        std::cerr << RED << "Error: Invalid event_status on event_loop" << RESET << std::endl;
                }

            }
        }
    }
}


//
// Created by rmore on 15/03/2023.
//

#include "../../includes/io/Multiplexer.h"

Multiplexer::Multiplexer() {}

Multiplexer::~Multiplexer() {}


void Multiplexer::event_loop(int server_socket_fd) {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << RED << "Error while creating epoll fd: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    struct epoll_event server_event = {};
    struct epoll_event request_event = {};

    server_event.events = EPOLLIN;
    server_event.data.fd = server_socket_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket_fd, &server_event) == -1) {
        std::cerr << RED << "Error while adding socket to epoll: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    struct epoll_event epoll_events[MAX_EPOLL_EVENTS];
    while (1) {
        int nfds = epoll_wait(epoll_fd, epoll_events, MAX_EPOLL_EVENTS, 3000);
        if (nfds == -1) {
            std::cerr << RED << "Error while waiting for epoll events: " << strerror(errno) << RESET << std::endl;
            continue;
        }

        for (int i = 0; i < nfds; i++) {
            if (epoll_events[i].data.fd == server_socket_fd) {

                int client_fd = Socket::setupClient(server_socket_fd);
                if (client_fd < 0) break;

                //TODO: Tornar a classe event apenas uma classe que guarda os dados
                Event *event = new Event(client_fd);
                request_event.data.ptr = event;
                request_event.events = EPOLLIN;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &request_event) == -1) {
                    std::cerr << RED << "Error while adding client to epoll: " << strerror(errno) << RESET << std::endl;
                    continue;
                }
            } else {
                //TODO: Criar uma classe EventHandler que vai receber a classe POJO event e vai fazer o processamento
                Event *event = (Event *) epoll_events[i].data.ptr;
                event->process_event();
                switch (event->getEventStatus()) {
                    case Reading:
                        request_event.events = EPOLLIN;
                        request_event.data.ptr = event;
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, event->getClientFd(), &request_event) < 0) {
                            std::cerr << RED << "Error while adding reading event to epoll: " << strerror(errno)
                                      << RESET << std::endl;
                            continue;
                        }
                        break;
                    case Writing:
                        request_event.events = EPOLLOUT;
                        request_event.data.ptr = event;
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, event->getClientFd(), &request_event) < 0) {
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
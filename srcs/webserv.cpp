#include "../includes/webserv.h"
#include "../includes/network/Socket.h"

void io_multiplexing_event_loop(int server_socket_fd) {
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

                Event *event = new Event(client_fd);
                request_event.data.ptr = event;
                request_event.events = EPOLLIN;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &request_event) == -1) {
                    std::cerr << RED << "Error while adding client to epoll: " << strerror(errno) << RESET << std::endl;
                    continue;
                }
            } else {
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

int main(int argc, char **argv, char **env) {
    signal(SIGPIPE, SIG_IGN);

    if (argc != 2) {
        std::cout << CYAN << "Usage :  ./webserv {PATH TO CONFIGURATION FILE}" << RESET << std::endl;
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    io_multiplexing_event_loop(Socket::setupServer(8080 + (rand() % 10)));

    return EXIT_SUCCESS;
}
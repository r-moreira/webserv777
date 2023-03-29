//
// Created by rmore on 15/03/2023.
//

#include "../../includes/network/Socket.h"

Socket::Socket() {}

Socket::~Socket() {}

int Socket::setupClient(int server_socket_fd) {
    struct sockaddr_in client_addr = {};

    Socket client_socket;

    socklen_t client_addr_len = sizeof(client_addr);

    int client_fd = accept(server_socket_fd, (struct sockaddr *) &client_addr, &client_addr_len);

    if (client_fd < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return -1;
        } else {
            std::cerr << RED << "Error while accepting client: " << strerror(errno) << RESET << std::endl;
            return -1;
        }
    }

    set_non_blocking(client_fd);
    return client_fd;
}

int Socket::setupServer(int port) {
    struct sockaddr_in sevr_addr = {};
    sevr_addr.sin_family = AF_INET;
    sevr_addr.sin_port = htons(port);
    sevr_addr.sin_addr.s_addr = INADDR_ANY;

    int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_fd < 0) {
        std::cerr << RED << "Error while opening server_fd: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *) &sevr_addr, sizeof(sevr_addr)) != 0) {
        std::cerr << RED << "Error while binding server_fd to address: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, SOMAXCONN) < 0) {
        std::cerr << RED << "Error while listening on server_fd: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << GREEN << "Server started to listen at port: " << port << RESET << std::endl;

    set_non_blocking(server_fd);
    return server_fd;
}

void Socket::set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << RED << "Error while getting flags: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        std::cerr << RED << "Error while setting flags: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

}


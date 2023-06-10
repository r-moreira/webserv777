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
            Logger::error("Error while accepting client: " + std::string(strerror(errno)));
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
        Logger::error("Error while creating server_fd: " + std::string(strerror(errno)));
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *) &sevr_addr, sizeof(sevr_addr)) != 0) {
        Logger::error("Error while binding server_fd to address: " + std::string(strerror(errno)));
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        Logger::error("Error while listening on server_fd: " + std::string(strerror(errno)));
        exit(EXIT_FAILURE);
    }

    Logger::info("Server started to listen on port" + ITOSTR(port));

    set_non_blocking(server_fd);
    return server_fd;
}

void Socket::set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        Logger::error("Error while getting flags: " + std::string(strerror(errno)));
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        Logger::error("Error while setting flags: " + std::string(strerror(errno)));
        exit(EXIT_FAILURE);
    }

}


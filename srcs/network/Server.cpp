//
// Created by rmore on 15/03/2023.
//

#include "../../includes/network/Server.h"
#include "../../includes/network/ServerBuilder.h"

class ServerBuilder;

Server::Server() {}

Server::~Server() {}

ServerBuilder Server::build() {
    return ServerBuilder();
}

std::string Server::getName() const {
    return _name;
}

int Server::getPort() const {
    return _port;
}

int Server::getFd() const {
    return _fd;
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
    {
        return os
                << "Server: " << std::endl
                << "Name: " << server.getName() << std::endl
                << "Port: " << server.getPort() << std::endl
                << "Server FD: " << server.getFd() << std::endl;
    }
}

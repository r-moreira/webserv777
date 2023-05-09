//
// Created by rmore on 15/03/2023.
//

#include "../../includes/network/Server.h"
#include "../../includes/network/ServerBuilder.h"

class ServerBuilder;

Server::Server() {
    this->_name = "webserv";
    this->_port = 80;
    this-> _fd = -1;
    this->_max_body_size = -1;
}

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

int Server::getMaxBodySize() const {
    return _max_body_size;
}

void Server::setMaxBodySize(int maxBodySize) {
    _max_body_size = maxBodySize;
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

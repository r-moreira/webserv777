//
// Created by rmore on 10/04/2023.
//

#include "../../includes/network/ServerBuilder.h"

ServerBuilder::ServerBuilder() {}

ServerBuilder::~ServerBuilder() {}

ServerBuilder &ServerBuilder::with_name(std::string name) {
    _server._name = name;
    return *this;
}

ServerBuilder &ServerBuilder::with_port(int port) {
    _server._port = port;
    return *this;
}

ServerBuilder &ServerBuilder::with_max_body_size(long max_body_size) {
    _server._max_body_size = max_body_size;
    return *this;
}

Server ServerBuilder::start() {
    _server._fd = Socket::setupServer(_server.getPort());
    return _server;
}

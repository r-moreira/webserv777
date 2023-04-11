//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_SERVERBUILDER_H
#define WEBSERV_SERVERBUILDER_H

#include "../webserv.h"
#include "Server.h"

class ServerBuilder {

private:
    Server _server;

public:
    ServerBuilder();

    ~ServerBuilder();

    ServerBuilder& with_name(std::string name);

    ServerBuilder& with_port(int port);

    Server start();
};


#endif //WEBSERV_SERVERBUILDER_H

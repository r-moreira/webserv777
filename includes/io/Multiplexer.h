//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_MULTIPLEXER_H
#define WEBSERV_MULTIPLEXER_H

#include "../webserv.h"
#include "../../includes/domain/EventHandler.h"
#include "../../includes/network/Socket.h"
#include "../network/Server.h"

class Multiplexer {
private:
    std::vector<Server> _servers;

public:
    Multiplexer(const std::vector<Server>& servers);

    ~Multiplexer();

    void event_loop();
};


#endif //WEBSERV_MULTIPLEXER_H
//std::vector<Server>::iterator it;
//for (it = _servers.begin(); it != _servers.end(); it++) {
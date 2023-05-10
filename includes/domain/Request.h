//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_REQUEST_H
#define WEBSERV_REQUEST_H

#include "Event.h"
#include "../../includes/parser/HttpRequestParser.h"
#include "EventStateHelper.h"
#include "../parser/RequestInfo.h"

class Request {

private:
    Event &_event;

public:
    Request(Event &event);

    ~Request();

    void read_request();

    void parse_request();

    void choose_server(std::vector<Server> servers);

    void validate_constraints();

    void choose_location();
};


#endif //WEBSERV_REQUEST_H

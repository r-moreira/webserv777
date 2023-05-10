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

    void handle_location();

    void validate_constraints();


private:
    static std::string repace_path_to_root(std::string request_uri, const std::string &request_path, const std::string &location_root);
};


#endif //WEBSERV_REQUEST_H

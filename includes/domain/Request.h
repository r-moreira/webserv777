//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_REQUEST_H
#define WEBSERV_REQUEST_H

#include "Event.h"
#include "../../includes/parser/HttpRequestParser.h"
#include "ErrorState.h"
#include "../parser/RequestInfo.h"
#include "../io/Read.h"

class Request {

private:
    Event &_event;
    Read _read;

public:
    Request(Event &event);

    ~Request();

    void read_request();

    void parse_request();

    void choose_server(std::vector<Server> servers);

    void choose_location();

    void validate_constraints();

    void define_response_state();

private:
    std::string path_to_root();

    bool is_directory(const std::string& path);
};


#endif //WEBSERV_REQUEST_H

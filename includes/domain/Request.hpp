//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include "../utils/Logger.hpp"
#include "Event.hpp"
#include "ErrorState.hpp"
#include "../io/Read.hpp"
#include "../../includes/parser/RequestParser.hpp"
#include "../model/RequestData.hpp"

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
    void _request_logger();

    std::string _path_to_root();

    bool _is_directory(const std::string& path);

    bool _is_index_exists_in_directory(const std::string& path, const std::string& root, const std::string& index);
};


#endif //WEBSERV_REQUEST_HPP

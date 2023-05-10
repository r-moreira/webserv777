//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_RESPONSE_H
#define WEBSERV_RESPONSE_H


#include "Event.h"
#include "EventStateHelper.h"

class Response {
private:
    Event &_event;

public:
    Response(Event &event);

    ~Response();

    void write_response_file();

    void write_file_response_headers();

    void write_default_error_page();

    void write_error_headers();

    void write_error_response();

private:
    std::string getFileHeaders(const std::string& file_path, size_t file_size);
    std::string getErrorHeaders();

    void read_requested_file();

    void write_requested_file();

    void write_headers(const std::string &headers);
};


#endif //WEBSERV_RESPONSE_H

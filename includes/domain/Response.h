//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_RESPONSE_H
#define WEBSERV_RESPONSE_H


#include "Event.h"

class Response {
private:
    Event &_event;

public:
    Response(Event &event);

    ~Response();

    void write_response_file();

    void write_response_headers();

    void write_error_page();

private:
    std::string getHeaders(const std::string& file_path, size_t file_size);
    std::string getErrorHeaders();

    void read_upload_file();

    void write_upload_file();

    void send_headers(const std::string &headers);
};


#endif //WEBSERV_RESPONSE_H

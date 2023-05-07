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

    void writting_file();

    void write_response_headers();

private:
    std::string getHeaders(const std::string& file_path, size_t file_size);

    void read_upload_file();

    void write_upload_file();
};


#endif //WEBSERV_RESPONSE_H

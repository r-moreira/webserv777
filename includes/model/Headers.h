//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_HEADERS_H
#define WEBSERV_HEADERS_H

#include "../webserv.h"

class Headers {

public:
    Headers();

    ~Headers();

    static std::string getErrorHeaders(event_http_status_enum_t status);

    static std::string getFileHeaders(const std::string &file_path, size_t file_size);
};


#endif //WEBSERV_HEADERS_H

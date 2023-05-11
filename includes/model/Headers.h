//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_HEADERS_H
#define WEBSERV_HEADERS_H

#include "../webserv.h"
#include "MimeTypes.h"

class Headers {
    MimeTypes _mime_types;

public:
    Headers();

    ~Headers();

    std::string getErrorHeaders(event_http_status_enum_t status);

    std::string getFileHeaders(const std::string &file_path, size_t file_size);

    std::string getRedirectionHeaders(const std::string &location);
};


#endif //WEBSERV_HEADERS_H

//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_HEADERS_H
#define WEBSERV_HEADERS_H

#include "../webserv.h"
#include "../utils/Logger.h"
#include "../domain/Event.h"
#include "MimeTypes.h"

class Headers {
    MimeTypes _mime_types;

public:
    Headers();

    ~Headers();

    std::string getErrorHeaders(Event::HttpStatus::event_http_status status);

    std::string getFileHeaders(const std::string &file_path, size_t file_size);

    std::string getRedirectionHeaders(const std::string &location);

    std::string getCreatedHeaders(const std::string &location);

    std::string getNoContentHeaders(const std::string &basicString);

    std::string getAutoIndexHeaders();

    std::string getCGIHeaders(Event::HttpStatus::event_http_status status);
};


#endif //WEBSERV_HEADERS_H

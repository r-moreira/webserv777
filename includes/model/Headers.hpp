//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_HEADERS_HPP
#define WEBSERV_HEADERS_HPP

#include "../webserv.hpp"
#include "../utils/Logger.hpp"
#include "../domain/Event.hpp"
#include "MimeTypes.hpp"

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


#endif //WEBSERV_HEADERS_HPP

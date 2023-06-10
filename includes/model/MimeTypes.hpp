//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_MIMETYPES_HPP
#define WEBSERV_MIMETYPES_HPP

#include "../webserv.hpp"

class MimeTypes {
private:
    std::map<std::string, std::string> _mime_types;

public:
    MimeTypes();
    ~MimeTypes();

    std::string get_mime_type(const std::string &extension);
};


#endif //WEBSERV_MIMETYPES_HPP

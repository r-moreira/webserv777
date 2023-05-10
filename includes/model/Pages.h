//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_PAGES_H
#define WEBSERV_PAGES_H

#include "../webserv.h"

class Pages {
public:
    Pages();

    ~Pages();

    static std::string get_default_error_page(event_http_status_enum_t status);
};


#endif //WEBSERV_PAGES_H

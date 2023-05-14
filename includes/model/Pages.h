//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_PAGES_H
#define WEBSERV_PAGES_H

#include "../webserv.h"
#include "../domain/Event.h"

class Pages {
public:
    Pages();

    ~Pages();

    static std::string get_default_error_page(Event::event_http_status status);
};


#endif //WEBSERV_PAGES_H

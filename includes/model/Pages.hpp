//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_PAGES_HPP
#define WEBSERV_PAGES_HPP

#include "../webserv.hpp"
#include "../domain/Event.hpp"

class Pages {
public:
    Pages();

    ~Pages();

    static std::string get_default_error_page(Event::HttpStatus::event_http_status status);
};


#endif //WEBSERV_PAGES_HPP

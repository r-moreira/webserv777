//
// Created by rmore on 08/05/2023.
//

#ifndef WEBSERV_ERRORSTATE_HPP
#define WEBSERV_ERRORSTATE_HPP

#include "../utils/Logger.hpp"
#include "Event.hpp"

class ErrorState {
public:
    ErrorState();

    static void throw_error_state(Event &event, Event::HttpStatus::event_http_status status);
    static bool is_error_state(Event &event);
};


#endif //WEBSERV_ERRORSTATE_HPP

//
// Created by rmore on 08/05/2023.
//

#ifndef WEBSERV_ERRORSTATE_H
#define WEBSERV_ERRORSTATE_H


#include "Event.h"

class ErrorState {
public:
    ErrorState();

    static void throw_error_state(Event &event, Event::HttpStatus::event_http_status status);
    static bool is_error_state(Event &event);
};


#endif //WEBSERV_ERRORSTATE_H

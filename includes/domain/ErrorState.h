//
// Created by rmore on 08/05/2023.
//

#ifndef WEBSERV_ERRORSTATE_H
#define WEBSERV_ERRORSTATE_H


#include "Event.h"

class ErrorState {
public:
    ErrorState();

    static void throw_error_state(Event &event, event_http_status_enum_t status);
    static bool is_error_state(Event &event);

    static void normalize_error_state(Event &event);
};


#endif //WEBSERV_ERRORSTATE_H

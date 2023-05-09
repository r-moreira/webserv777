//
// Created by rmore on 08/05/2023.
//

#ifndef WEBSERV_EVENTSTATEHELPER_H
#define WEBSERV_EVENTSTATEHELPER_H


#include "Event.h"

class EventStateHelper {
public:
    EventStateHelper();

    static void error_state(Event &event, event_http_status_enum_t status);
};


#endif //WEBSERV_EVENTSTATEHELPER_H

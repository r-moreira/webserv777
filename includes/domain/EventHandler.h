//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_EVENTHANDLER_H
#define WEBSERV_EVENTHANDLER_H

#include "../webserv.h"
#include "Event.h"
#include "Request.h"
#include "Response.h"
#include "File.h"

/*
 * TODO: Encapsular métodos dentro de classes
 */

class EventHandler {

private:
    Event &_event;
    Request _request;
    Response _response;
    File _file;

public:
    EventHandler(Event &event);

    ~EventHandler();

    void process_event();

    const Event &getEvent() const;

    void setEvent(const Event &event);

private:
    void print_event_status();
};


#endif //WEBSERV_EVENTHANDLER_H

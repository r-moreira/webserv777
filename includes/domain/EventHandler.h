//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_EVENTHANDLER_H
#define WEBSERV_EVENTHANDLER_H

#include "../webserv.h"
#include "Event.h"
#include "Request.h"

/*
 * TODO: Encapsular métodos dentro de classes
 */

class EventHandler {
private:
    Event &_event;
    Request _request;

public:
    EventHandler(Event &event);

    ~EventHandler();

    void process_event();

    void write_response_headers();

    void open_file();

    void read_upload_file();

    void write_upload_file();

    void upload_file();

    std::string getHeaders(const std::string& file_path, size_t file_size);

    const Event &getEvent() const;

    void setEvent(const Event &event);
};


#endif //WEBSERV_EVENTHANDLER_H

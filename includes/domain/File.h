//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_FILE_H
#define WEBSERV_FILE_H


#include "Event.h"
#include "EventStateHelper.h"

class File {

private:
    Event &_event;

public:
    File(Event &event);

    ~File();

    void open_response_file();
};


#endif //WEBSERV_FILE_H

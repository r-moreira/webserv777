//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_FILE_H
#define WEBSERV_FILE_H


#include "Event.h"

class File {

private:
    Event &_event;

public:
    File(Event &event);

    ~File();

    void open_file();
};


#endif //WEBSERV_FILE_H

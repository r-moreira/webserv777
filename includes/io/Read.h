//
// Created by rmore on 09/04/2023.
//

#ifndef WEBSERV_READ_H
#define WEBSERV_READ_H

#include "../webserv.h"

class Read {
private:
    Event _event;

public:
    Read();
    ~Read();
    Read(Event &event);
};


#endif //WEBSERV_READ_H

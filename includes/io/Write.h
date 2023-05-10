//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_WRITE_H
#define WEBSERV_WRITE_H

#include "../domain/Event.h"
#include "../domain/ErrorState.h"

class Write {

private:
    Event &_event;

public:
    Write(Event &event);

    ~Write();

};


#endif //WEBSERV_WRITE_H

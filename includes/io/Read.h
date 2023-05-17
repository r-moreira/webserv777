//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_READ_H
#define WEBSERV_READ_H


#include "../domain/Event.h"
#include "../domain/ErrorState.h"

class Read {

private:
    Event &_event;

public:
    Read(Event &event);

    ~Read();

    void read_request();

    void read_file();

    void read_upload_file();
};


#endif //WEBSERV_READ_H

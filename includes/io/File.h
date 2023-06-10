//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_FILE_H
#define WEBSERV_FILE_H

#include "../utils/Logger.h"
#include "../domain/Event.h"
#include "../domain/ErrorState.h"

class File {

private:
    Event &_event;

public:
    File(Event &event);

    ~File();

    void open_file();

    void create_file();

    void delete_file();
};


#endif //WEBSERV_FILE_H

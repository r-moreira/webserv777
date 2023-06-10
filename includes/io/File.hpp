//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_FILE_HPP
#define WEBSERV_FILE_HPP

#include "../utils/Logger.hpp"
#include "../domain/Event.hpp"
#include "../domain/ErrorState.hpp"

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


#endif //WEBSERV_FILE_HPP

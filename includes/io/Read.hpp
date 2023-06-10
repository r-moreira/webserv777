//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_READ_HPP
#define WEBSERV_READ_HPP

#include "../utils/Logger.hpp"
#include "../domain/Event.hpp"
#include "../domain/ErrorState.hpp"

class Read {

private:
    Event &_event;

public:
    Read(Event &event);

    ~Read();

    void read_request();

    void read_file();

    void read_body_content();
};


#endif //WEBSERV_READ_HPP

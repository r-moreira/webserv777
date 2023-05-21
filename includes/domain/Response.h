//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_RESPONSE_H
#define WEBSERV_RESPONSE_H


#include "Event.h"
#include "../io/File.h"
#include "../io/Read.h"
#include "../io/Write.h"

class Response {
private:
    Event &_event;
    File _file;
    Read _read;
    Write _write;

public:
    Response(Event &event);

    ~Response();

    void send_response_file();

    void send_error_response();

    void send_is_directory_response();

    void send_redirection();

    void send_upload_response();

    void send_delete_response();

    void send_auto_index();
};


#endif //WEBSERV_RESPONSE_H

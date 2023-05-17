//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_WRITE_H
#define WEBSERV_WRITE_H

#include "../domain/Event.h"
#include "../domain/ErrorState.h"
#include "../../includes/model/Headers.h"
#include "../../includes/model/Pages.h"

class Write {

private:
    Event &_event;
    Headers _headers;

public:
    Write(Event &event);

    ~Write();

    void write_error_headers();

    void write_file_response_headers();

    void write_redirection_headers();

    void write_requested_file();

    void write_default_error_page();

    void write_created_headers();

    void write_uploaded_file();

private:
    void write_headers(const std::string &headers);
};


#endif //WEBSERV_WRITE_H

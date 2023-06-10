//
// Created by rmore on 10/04/2023.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include "../utils/Logger.hpp"
#include "Event.hpp"
#include "../io/File.hpp"
#include "../io/Read.hpp"
#include "../io/Write.hpp"
#include "../model/AutoIndex.hpp"
#include "../cgi/Exec.hpp"
#include "../cgi/ExecPython.hpp"
#include "../cgi/Environment.hpp"

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

    void send_redirection_response();

    void send_upload_response();

    void send_delete_response();

    void send_auto_index_response();

    void send_cgi_response();

private:
    void _response_logger();

    void _clear_cgi_exec();
};


#endif //WEBSERV_RESPONSE_HPP

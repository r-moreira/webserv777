//
// Created by rmore on 10/05/2023.
//

#ifndef WEBSERV_WRITE_HPP
#define WEBSERV_WRITE_HPP

#include "../utils/Logger.hpp"
#include "../domain/Event.hpp"
#include "../domain/ErrorState.hpp"
#include "../../includes/model/Headers.hpp"
#include "../../includes/model/Pages.hpp"

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

    void write_auto_index_headers();

    void write_requested_file();

    void write_default_error_page();

    void write_created_headers();

    void write_remaining_read_buffer_to_file();

    void write_upload_file();

    void write_no_content_headers();

    void write_auto_index_page(const std::string& auto_index_page);

    void write_cgi_headers();

    void write_body_to_cgi();

    void write_cgi_content();

private:
    void _write_headers(const std::string &headers);
};


#endif //WEBSERV_WRITE_HPP

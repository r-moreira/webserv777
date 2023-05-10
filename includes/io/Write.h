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

    void write_error_headers();

    void write_file_response_headers();

private:
    void write_headers(const std::string &headers);
    std::string getErrorHeaders();

    std::string getFileHeaders(const std::string &file_path, size_t file_size);
;
};


#endif //WEBSERV_WRITE_H

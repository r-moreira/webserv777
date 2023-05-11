//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/Response.h"

Response::Response(Event &event): _event(event), _file(event), _read(event), _write(event) {}

Response::~Response() {}

void Response::send_response_file() {
    std::cout << MAGENTA << "Send file response" << RESET << std::endl;

    _file.open_file();
    _read.read_file();
    _write.write_file_response_headers();
    _write.write_requested_file();
}

void Response::send_redirection() {
    std::cout << MAGENTA << "Send redirection response" << RESET << std::endl;

    _write.write_redirection_headers();
    _event.setEventStatus(Ended);
}

void Response::send_is_directory_response() {
    std::cout << MAGENTA << "Send directory error response" << RESET << std::endl;

    if (!_event.getServer().getDirectoryRequestPage().empty()) {
        this->_event.setErrorResponse(true);
        this->_event.clear_file_info();
        this->_event.setFilePath(_event.getServer().getDirectoryRequestPage());
        _file.open_file();
        _read.read_file();
        _write.write_error_headers();
        _write.write_requested_file();
    }
    else {
        _write.write_error_headers();
        _write.write_default_error_page();
    }

    _event.setEventStatus(Ended);
}

void Response::send_error_response() {
    std::cout << MAGENTA << "Send error response" << RESET << std::endl;

    try {
        std::string error_page_path = _event.getServer().getErrorPages().at(_event.getHttpStatus());
        this->_event.setErrorResponse(true);
        this->_event.clear_file_info();
        this->_event.setFilePath(error_page_path);
        _file.open_file();
        _read.read_file();
        _write.write_error_headers();
        _write.write_requested_file();
    } catch (std::out_of_range &e) {
        _write.write_error_headers();
        _write.write_default_error_page();
    }

    _event.setEventStatus(Ended);
}

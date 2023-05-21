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
    _event.setEventStatus(Event::Ended);
}

void Response::send_upload_response() {
    std::cout << MAGENTA << "Send upload response" << RESET << std::endl;

    if (!this->_event.isFileOpened()) {
        std::string upload_path;
        std::string file_name;

        upload_path = !this->_event.getLocation().getUploadPath().empty()
                      ? this->_event.getLocation().getUploadPath()
                      : this->_event.getServer().getUploadPath();

        if (upload_path.empty()) {
            std::cout << RED << "Upload path not found" << RESET << std::endl;
            this->_event.setHttpStatus(Event::INTERNAL_SERVER_ERROR);
            send_error_response();
            return;
        }

        upload_path = upload_path[upload_path.length() - 1] == '/' ? upload_path : upload_path + "/";

        if (this->_event.getRequest().getContentDisposition().empty() ||
            this->_event.getRequest().getContentDisposition().find("filename=\"") == std::string::npos) {
            std::cout << RED << "Content-Disposition header not found" << RESET << std::endl;

            this->_event.setHttpStatus(Event::BAD_REQUEST);
            send_error_response();
            return;
        }

        size_t file_name_start = this->_event.getRequest().getContentDisposition().find("filename=\"") + 10;
        size_t file_name_end = this->_event.getRequest().getContentDisposition().find('\"', file_name_start);

        file_name = this->_event.getRequest().getContentDisposition().substr(file_name_start, file_name_end - file_name_start);

        this->_event.setFilePath(upload_path + file_name);
        this->_event.setRemainingFileUploadBytes(this->_event.getRequest().getFileUploadRemainingBytes());
        this->_event.setFileReadLeft(this->_event.getRequest().getFileUploadRemainingBytes());
        _file.create_file();
    }

    _write.write_remaining_read_buffer_to_file();

    if (this->_event.getRemainingFileUploadBytes() != 0) {
        _read.read_upload_file();
        _write.write_upload_file();
    }

    if (this->_event.getRemainingFileUploadBytes() == 0 || this->_event.getFileReadLeft() == 0) {
        _write.write_created_headers();
    }
}

void Response::send_delete_response() {
    std::cout << MAGENTA << "Send delete response" << RESET << std::endl;

    _file.delete_file();
    _write.write_no_content_headers();
}


void Response::send_is_directory_response() {
    std::cout << MAGENTA << "Send directory error response" << RESET << std::endl;

    if (!_event.getServer().getDirectoryRequestPage().empty() || !_event.getLocation().getDirectoryRequestPage().empty()) {
        this->_event.setErrorResponse(true);
        this->_event.clear_file_info();

        std::string directory_page_path = !_event.getLocation().getDirectoryRequestPage().empty()
                                                ? _event.getLocation().getDirectoryRequestPage()
                                                : _event.getServer().getDirectoryRequestPage();

        this->_event.setFilePath(directory_page_path);
        _file.open_file();
        _read.read_file();
        _write.write_error_headers();
        _write.write_requested_file();
    }
    else {
        _write.write_error_headers();
        _write.write_default_error_page();
    }

    _event.setEventStatus(Event::Ended);
}

void Response::send_error_response() {
    std::cout << MAGENTA << "Send error response" << RESET << std::endl;

    bool server_has_error_page;
    bool location_has_error_page;

    try {
        std::string server_error_page_path = _event.getServer().getErrorPages().at(_event.getHttpStatus());
        server_has_error_page = true;
    } catch (std::out_of_range &e) {
        server_has_error_page = false;
    }

    try {
        std::string location_error_page_path = _event.getLocation().getErrorPages().at(_event.getHttpStatus());
        location_has_error_page = true;
    } catch (std::out_of_range &e) {
        location_has_error_page = false;
    }

    if (server_has_error_page || location_has_error_page) {
        this->_event.setErrorResponse(true);
        this->_event.clear_file_info();

        std::string error_page_path = location_has_error_page
                                                ? _event.getLocation().getErrorPages().at(_event.getHttpStatus())
                                                : _event.getServer().getErrorPages().at(_event.getHttpStatus());

        this->_event.setFilePath(error_page_path);
        _file.open_file();
        _read.read_file();
        _write.write_error_headers();
        _write.write_requested_file();
    }
    else {
        _write.write_error_headers();
        _write.write_default_error_page();
    }

    _event.setEventStatus(Event::Ended);
}
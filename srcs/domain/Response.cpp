//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/Response.hpp"

Response::Response(Event &event) : _event(event), _file(event), _read(event), _write(event) {}

Response::~Response() {}

void Response::send_response_file() {
    Logger::debug("Send response file");

    _file.open_file();
    _read.read_file();
    _write.write_file_response_headers();
    _write.write_requested_file();

    if (_event.getEventStatus() == Event::Status::Ended) {
        _response_logger();
    }
}

void Response::send_redirection_response() {
    Logger::debug("Send redirection response");

    _write.write_redirection_headers();
    _event.setEventStatus(Event::Status::Ended);
    _response_logger();
}

void Response::send_upload_response() {
    Logger::debug("Send upload response");

    if (!this->_event.isFileOpened()) {
        std::string upload_path;
        std::string file_name;

        upload_path = !this->_event.getLocation().getUploadPath().empty()
                      ? this->_event.getLocation().getUploadPath()
                      : this->_event.getServer().getUploadPath();

        if (upload_path.empty()) {
            Logger::warning("Upload path not found");
            this->_event.setHttpStatus(Event::HttpStatus::INTERNAL_SERVER_ERROR);
            send_error_response();
            return;
        }

        upload_path = upload_path[upload_path.length() - 1] == '/' ? upload_path : upload_path + "/";

        if (this->_event.getRequest().getContentDisposition().empty() ||
            this->_event.getRequest().getContentDisposition().find("filename=\"") == std::string::npos) {
            Logger::warning("Content-Disposition header not found");

            this->_event.setHttpStatus(Event::HttpStatus::BAD_REQUEST);
            send_error_response();
            return;
        }

        size_t file_name_start = this->_event.getRequest().getContentDisposition().find("filename=\"") + 10;
        size_t file_name_end = this->_event.getRequest().getContentDisposition().find('\"', file_name_start);

        file_name = this->_event.getRequest().getContentDisposition().substr(file_name_start,
                                                                             file_name_end - file_name_start);

        this->_event.setFilePath(upload_path + file_name);
        this->_event.setRemainingFileBytes(this->_event.getRequest().getBodyRemainingBytes());
        this->_event.setFileReadLeft(this->_event.getRequest().getBodyRemainingBytes());
        _file.create_file();
    }

    _write.write_remaining_read_buffer_to_file();

    if (this->_event.getRemainingFileBytes() != 0) {
        _read.read_body_content();
        _write.write_upload_file();
    }

    if (this->_event.getRemainingFileBytes() == 0 || this->_event.getFileReadLeft() == 0) {
        _write.write_created_headers();
        _response_logger();
    }
}

void Response::send_delete_response() {
    Logger::debug("Send delete response");

    _file.delete_file();
    _write.write_no_content_headers();
    _response_logger();
}

void Response::send_auto_index_response() {
    Logger::debug("Send auto index response");

    std::string auto_index_page = AutoIndex::pageGenerator(_event.getFilePath(), _event.getRequest().getUri(),
                                                           _event.getServer().getPort());
    _write.write_auto_index_headers();
    _write.write_auto_index_page(auto_index_page);
    _response_logger();
}

void Response::send_cgi_response() {
    Logger::debug("Send CGI response");

    if (!this->_event.isCgiSet()) {
        Environment env;

        this->_event.setEnvp(env.getCgiEnvp(this->_event));

        Logger::trace("CGI envp:");
        std::stringstream log;
        for (int i = 0; i < Environment::ENV_VARIABLES_SIZE; i++) {
            log << this->_event.getEnvp()[i] << std::endl;
        }
        Logger::trace(log.str());

        std::string cgi_bin = "python3";
        if (!this->_event.getLocation().getCgiBin().empty()) {
            cgi_bin = this->_event.getLocation().getCgiBin();
        }

        this->_event.setCgiPath(strdup(_event.getLocation().getCgiPath().c_str()));
        char *const cmd[] = {(char *) cgi_bin.c_str(),  this->_event.getCgiPath(), NULL};

        if (cgi_bin == "python3") {
            this->_event.setCgi(new ExecPython(cmd, this->_event.getEnvp()));
        } else {
            this->_event.setCgi(new Exec((char *)this->_event.getLocation().getCgiBin().c_str(), cmd, this->_event.getEnvp()));
        }


        if (this->_event.getRequest().getMethod() == "POST") {
            this->_event.setRemainingFileBytes(this->_event.getRequest().getBodyRemainingBytes());
            this->_event.setFileReadLeft(this->_event.getRequest().getBodyRemainingBytes());
            this->_event.setRemainingFileBytes(this->_event.getRequest().getBodyRemainingBytes());
            this->_event.setIsCgiSet(true);
        }
    }

    if (this->_event.getRequest().getMethod() == "GET") {
        this->_event.getCgi()->start(1);
        this->_event.setHttpStatus(_event.convert_int_to_http_status(this->_event.getCgi()->getHttpStatusCode()));

        Logger::debug("CGI status: " + ITOSTR(this->_event.getHttpStatus()));

        if (this->_event.getHttpStatus() != Event::HttpStatus::OK) {
            _clear_cgi_exec();
            send_error_response();
            return;
        }

        _write.write_cgi_headers();
        this->_event.setCgiFdOut(this->_event.getCgi()->getStdOut());
        _write.write_cgi_content();
        _clear_cgi_exec();
        _response_logger();

    } else if (this->_event.getRequest().getMethod() == "POST") {
        _write.write_body_to_cgi();

        if ((this->_event.getRemainingFileBytes() == 0 || this->_event.getFileReadLeft() == 0)) {
            Logger::trace("Starting CGI");

            this->_event.getCgi()->start(this->_event.getServerCgiFdOut());
            this->_event.setHttpStatus(_event.convert_int_to_http_status(this->_event.getCgi()->getHttpStatusCode()));

            Logger::debug("CGI status: " + ITOSTR(this->_event.getHttpStatus()));

            if (this->_event.getHttpStatus() != Event::HttpStatus::OK) {
                _clear_cgi_exec();
                send_error_response();
                return;
            }

            _write.write_cgi_headers();
            this->_event.setCgiFdOut(this->_event.getCgi()->getStdOut());
            _write.write_cgi_content();

            _clear_cgi_exec();
            _response_logger();
        }

    } else {
        this->_event.setHttpStatus(Event::HttpStatus::FORBIDDEN);
        send_error_response();
    }
}

void Response::send_is_directory_response() {
    Logger::debug("Send is directory response");

    if (!_event.getServer().getDirectoryRequestPage().empty() ||
        !_event.getLocation().getDirectoryRequestPage().empty()) {
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
    } else {
        _write.write_error_headers();
        _write.write_default_error_page();
    }

    _response_logger();
    _event.setEventStatus(Event::Status::Ended);
}

void Response::send_error_response() {
    Logger::debug("Send error response");

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
    } else {
        _write.write_error_headers();
        _write.write_default_error_page();
    }

    _response_logger();
    _event.setEventStatus(Event::Status::Ended);
}

void Response::_response_logger() {
    std::stringstream log;

    if (this->_event.getHttpStatus() != 499) {
        log << this->_event.getRequestLogInfo() << " | " << MAGENTA << "Status" << RESET << ": " << this->_event.getHttpStatus();
        Logger::info(log.str());
    }
}

void Response::_clear_cgi_exec() {
    Environment::freeCgiEnvp(this->_event.getEnvp());
    if (_event.isCgiSet() && _event.getEventStatus() == Event::Status::Ended) {
        delete this->_event.getCgi();
        free(this->_event.getCgiPath());
        close(_event.getCgiFdOut());
    }
}
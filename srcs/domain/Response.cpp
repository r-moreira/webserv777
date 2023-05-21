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

void Response::send_redirection_response() {
    std::cout << MAGENTA << "Send redirection response" << RESET << std::endl;

    _write.write_redirection_headers();
    _event.setEventStatus(Event::Status::Ended);
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

//TODO: Retornar pagina de auto index
void Response::send_auto_index_response() {
    std::cout << MAGENTA << "Send auto index response" << RESET << std::endl;

    _write.write_auto_index_headers();
    std::string auto_index_page = "<!DOCTYPEhtml><html><head><metacharset=\"UTF-8\"><metahttp-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><metaname=\"viewport\"content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\"><title>Server Default Autoindex</title><style>html,body{width:100%;height:100%;margin:0;padding:0;}body{display:flex;align-items:center;justify-content:center;background-color:#424;font-size:14px;}h3{font-size:60px;color:#eee;text-align:center;padding-top:30px;font-weight:normal;}</style></head><body><h3>Auto Index Temp Page</h3></body></html>";
    if (send(_event.getClientFd(), auto_index_page.c_str(), auto_index_page.size(), 0) < 0) {
        std::cerr << RED << "Error while writing error page to client: " << strerror(errno) << RESET << std::endl;
    }

    _event.setEventStatus(Event::Status::Ended);
}

//TODO: Retornar CGI
void Response::send_cgi_response() {
    std::cout << MAGENTA << "Send CGI response" << RESET << std::endl;

    std::string cgi_tmp_headers = "HTTP/1.1 200 Ok\r\nContent-Type: text/html\r\n\r\n";

    std::cout << CYAN << "Send auto tmp CGI headers:" << RESET << std::endl;

    if (send(_event.getClientFd(), cgi_tmp_headers.c_str(), cgi_tmp_headers.size(), 0) < 0) {
        std::cerr << RED << "Error while writing status header to client: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::INTERNAL_SERVER_ERROR);
        return;
    }
    _event.setHeaderSent(true);

    std::string get_cgi_page = "<!DOCTYPEhtml><html><head><metacharset=\"UTF-8\"><metahttp-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><metaname=\"viewport\"content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\"><title>Server Default GET GGI</title><style>html,body{width:100%;height:100%;margin:0;padding:0;}body{display:flex;align-items:center;justify-content:center;background-color:#424;font-size:14px;}h3{font-size:60px;color:#eee;text-align:center;padding-top:30px;font-weight:normal;}</style></head><body><h3>GET CGI Temp Page</h3></body></html>";
    std::string post_cgi_page = "<!DOCTYPEhtml><html><head><metacharset=\"UTF-8\"><metahttp-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><metaname=\"viewport\"content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\"><title>Server Default POST GGI</title><style>html,body{width:100%;height:100%;margin:0;padding:0;}body{display:flex;align-items:center;justify-content:center;background-color:#424;font-size:14px;}h3{font-size:60px;color:#eee;text-align:center;padding-top:30px;font-weight:normal;}</style></head><body><h3>POST CGI Temp Page</h3></body></html>";

    std::string cgi_page = _event.getRequest().getMethod() == "GET" ? get_cgi_page : post_cgi_page;

    if (send(_event.getClientFd(), cgi_page.c_str(), cgi_page.size(), 0) < 0) {
        std::cerr << RED << "Error while writing error page to client: " << strerror(errno) << RESET << std::endl;
    }

    _event.setEventStatus(Event::Status::Ended);
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

    _event.setEventStatus(Event::Status::Ended);
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

    _event.setEventStatus(Event::Status::Ended);
}
//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/Response.h"

Response::Response(Event &event) : _event(event), _file(event), _read(event), _write(event) {}

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
            this->_event.setHttpStatus(Event::HttpStatus::INTERNAL_SERVER_ERROR);
            send_error_response();
            return;
        }

        upload_path = upload_path[upload_path.length() - 1] == '/' ? upload_path : upload_path + "/";

        if (this->_event.getRequest().getContentDisposition().empty() ||
            this->_event.getRequest().getContentDisposition().find("filename=\"") == std::string::npos) {
            std::cout << RED << "Content-Disposition header not found" << RESET << std::endl;

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
    }
}

void Response::send_delete_response() {
    std::cout << MAGENTA << "Send delete response" << RESET << std::endl;

    _file.delete_file();
    _write.write_no_content_headers();
}

void Response::send_auto_index_response() {
    std::cout << MAGENTA << "Send auto index response" << RESET << std::endl;

    std::string auto_index_page = AutoIndex::pageGenerator(_event.getFilePath(), _event.getRequest().getUri(),
                                                           _event.getServer().getPort());
    _write.write_auto_index_headers();
    _write.write_auto_index_page(auto_index_page);
}

void Response::send_cgi_response() {
    std::cout << MAGENTA << "Send CGI response" << RESET << std::endl;

    if (!this->_event.isCgiSet()) {
        Environment env;

        this->_event.setEnvp(env.getCgiEnvp(this->_event));
        std::cout << CYAN << "CGI envp:" << RESET << std::endl;
        for (int i = 0; i < Environment::ENV_VARIABLES_SIZE; i++) {
            std::cout << CYAN <<  this->_event.getEnvp()[i] << RESET << std::endl;
        }

        this->_event.setCgiPath(strdup(_event.getLocation().getCgiPath().c_str()));
        char *const cmd[] = {(char *) "python3",  this->_event.getCgiPath(), NULL};

        this->_event.setCgi(new ExecPython(cmd, this->_event.getEnvp()));

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

        std::cout << YELLOW << "CGI status: " << this->_event.getHttpStatus() << RESET << std::endl;

        if (this->_event.getHttpStatus() != Event::HttpStatus::OK) {
            clear_cgi_exec();
            send_error_response();
            return;
        }

        _write.write_cgi_headers();
        this->_event.setCgiFdOut(this->_event.getCgi()->getStdOut());
        _write.write_cgi_content();
        clear_cgi_exec();

    } else if (this->_event.getRequest().getMethod() == "POST") {
        _write.write_remaining_read_buffer_to_cgi();

        if ((this->_event.getRemainingFileBytes() == 0 || this->_event.getFileReadLeft() == 0)) {
            std::cout << MAGENTA << "Starting CGI" << RESET << std::endl;
            this->_event.getCgi()->start(this->_event.getServerCgiFdOut());

            this->_event.setHttpStatus(_event.convert_int_to_http_status(this->_event.getCgi()->getHttpStatusCode()));

            std::cout << YELLOW << "CGI status: " << this->_event.getHttpStatus() << RESET << std::endl;

            if (this->_event.getHttpStatus() != Event::HttpStatus::OK) {
                clear_cgi_exec();
                send_error_response();
                return;
            }

            _write.write_cgi_headers();
            this->_event.setCgiFdOut(this->_event.getCgi()->getStdOut());
            _write.write_cgi_content();
            clear_cgi_exec();
        }

    } else {
        this->_event.setHttpStatus(Event::HttpStatus::FORBIDDEN);
        send_error_response();
    }
}


//Tem algum bug aqui -> Tentar outras alternativas como read e write ao mesmo tempo, reservar um vector com o tamanho do body e ir lendo e escrevendo
//if (this->_event.getRemainingFileBytes() != 0 && !this->_event.isCgiExec()) {
//_read.read_body_content();
//_write.write_body_to_cgi(); //Trocar STDOUT de dentro do método para o FD do STDIN do CGI
//}

void Response::clear_cgi_exec() {
    Environment::freeCgiEnvp(this->_event.getEnvp());
    if (_event.isCgiSet() && _event.getEventStatus() == Event::Status::Ended) {
        delete this->_event.getCgi();
        free(this->_event.getCgiPath());
        close(_event.getCgiFdOut());
    }
}


void Response::send_is_directory_response() {
    std::cout << MAGENTA << "Send directory error response" << RESET << std::endl;

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
    } else {
        _write.write_error_headers();
        _write.write_default_error_page();
    }

    _event.setEventStatus(Event::Status::Ended);
}
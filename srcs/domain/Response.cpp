//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/Response.h"

Response::Response(Event &event): _event(event), _file(event) {}

Response::~Response() {}

std::string file_extension[] = {
        "aac", "avi", "bmp", "css", "gif", "ico", "js",
        "json", "mp3", "mp4", "otf", "png", "php", "rtf",
        "svg", "txt", "webm", "webp", "woff", "woff", "zip",
        "html", "htm", "jpeg", "jpg",
};

std::string content_type[] = {
        "Content-Type: audio/aac\r\n\r\n",
        "Content-Type: video/x-msvideo\r\n\r\n",
        "Content-Type: image/bmp\r\n\r\n",
        "Content-Type: text/css\r\n\r\n",
        "Content-Type: image/gif\r\n\r\n",
        "Content-Type: image/vnd.microsoft.icon\r\n\r\n",
        "Content-Type: text/javascript\r\n\r\n",
        "Content-Type: application/json\r\n\r\n",
        "Content-Type: audio/mpeg\r\n\r\n",
        "Content-Type: video/mp4\r\n\r\n",
        "Content-Type: font/otf\r\n\r\n",
        "Content-Type: image/png\r\n\r\n",
        "Content-Type: application/x-httpd-php\r\n\r\n",
        "Content-Type: application/rtf\r\n\r\n",
        "Content-Type: image/svg+xml\r\n\r\n",
        "Content-Type: text/plain\r\n\r\n",
        "Content-Type: video/webm\r\n\r\n",
        "Content-Type: video/webp\r\n\r\n",
        "Content-Type: font/woff\r\n\r\n",
        "Content-Type: font/woff2\r\n\r\n",
        "Content-Type: application/zip\r\n\r\n",
        "Content-Type: text/html\r\n\r\n",
        "Content-Type: text/html\r\n\r\n",
        "Content-Type: image/jpeg\r\n\r\n",
        "Content-Type: image/jpeg\r\n\r\n",
};

void Response::write_response_file() {
    _file.open_response_file();
    read_requested_file();
    write_file_response_headers();
    write_requested_file();
}

void Response::write_file_response_headers() {
    if (EventStateHelper::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    std::cout << MAGENTA << "Writing file response headers" << RESET << std::endl;

    write_headers(getFileHeaders(_event.getFilePath(), _event.getFileSize()));
    if (EventStateHelper::is_error_state(this->_event)) return;
}

void Response::read_requested_file() {
    if (EventStateHelper::is_error_state(this->_event)) return;

    std::cout << MAGENTA << "Reading request file: " << this->_event.getFilePath() << RESET << std::endl;

    size_t read_size;
    if (_event.getFileSize() > FILE_READ_CHUNK_SIZE) {
        read_size = _event.getFileReadLeft() > FILE_READ_CHUNK_SIZE ? FILE_READ_CHUNK_SIZE : _event.getFileReadLeft();
    } else {
        read_size = _event.getFileSize();
    }

    std::cout << YELLOW << "Read Data Size: " << read_size << RESET << std::endl;
    size_t chunk_bytes = fread((void *) this->_event.getFileReadChunkBuffer(), 1, read_size, _event.getFile());

    if (ferror(_event.getFile())) {
        std::cerr << RED << "Error while reading file: " << strerror(errno) << RESET << std::endl;

        if (errno == EISDIR) {
            std::cerr << RED << "Redirecionado para página de erro de diretório" << RESET << std::endl;
            _event.setEventSubStatus(WritingDirectoryResponse);
            _event.setHttpStatus(FORBIDDEN);
            return;
        }
        _event.setEventStatus(Ended);
        return;
    }

    _event.setFileReadBytes(_event.getFileReadBytes() + chunk_bytes);
    std::cout << YELLOW << "Readed Data Size: " << chunk_bytes << RESET << std::endl;

    _event.setFileReadLeft(_event.getFileSize() - _event.getFileReadBytes());
    std::cout << YELLOW << "Read Left: " << _event.getFileReadLeft() << RESET << std::endl;

    this->_event.setFileChunkReadBytes(chunk_bytes);
}

void Response::write_requested_file() {
    if (EventStateHelper::is_error_state(this->_event)) return;

    long bytes_sent = send(_event.getClientFd() , _event.getFileReadChunkBuffer(), _event.getFileChunkReadBytes(), 0);
    if (bytes_sent < 0) {
        std::cerr << RED << "Error while writing to client: " << strerror(errno) << RESET << std::endl;
        _event.setEventStatus(Ended);
        return;
    }

    std::cout << YELLOW << "Transmitted Data Size " << bytes_sent << " Bytes." << RESET << std::endl;

    if (_event.getFileReadLeft() <= 0) {
        _event.setEventStatus(Ended);
        std::cout << GREEN << "File Transfer Complete." << RESET << std::endl;
    }
}


std::string Response::getFileHeaders(const std::string& file_path, size_t file_size) {
    char size_t_byte_buffer[25] = {};
    std::string headers = "HTTP/1.1 200 Ok\r\n";

    if (snprintf(size_t_byte_buffer, 25, "%lu", file_size) < 0) {
        std::cerr << RED << "Error while formatting file size: " << strerror(errno) << RESET << std::endl;
        //stop connection, display error page
    }
    std::string content_length = "Content-Length: " + std::string(size_t_byte_buffer) + "\r\n";

    std::string extension = file_path.substr(file_path.find_last_of('.') + 1);
    for (int i = 0; i < 25; i++) {
        if (extension == file_extension[i]) {
            headers += content_length;
            headers += content_type[i];
            return headers;
        }
    }
    return "HTTP/1.1 200 Ok\r\n" + content_length + "Content-Type: text/html\r\n\r\n";
}

//TODO:: Checar se a pagina de diretório está configurada:
// caso exista retorna-la,
// caso contratio, retornar paginas de erro padrão do webserv
void Response::write_is_directory_response() {
    std::cout << MAGENTA << "Send directory error response" << RESET << std::endl;

    // if existe uma pagina de erro de diretório configurada
        // read_error_configured_page(path_da_config_da_pagina_diretório);
        // write_error_headers();
        // write_configured_error_page();
    // else

    write_error_headers();
    write_default_error_page();

    _event.setEventStatus(Ended);
}

//TODO:: Checar se existem paginas de erro padrão configuradas:
// caso exista retorna-las,
// caso contratio, retornar paginas de erro padrão do webserv
void Response::write_error_response() {
    // if existe uma pagina de erro configurada para o erro atual
        // read_error_configured_page(path_da_config_da_pagina_de_erro)
        // write_error_headers();
        // write_configured_error_page();

    // else
    write_error_headers();
    write_default_error_page();

    _event.setEventStatus(Ended);
}

void Response::write_error_headers() {
    if (this->_event.isHeaderSent()) return;

    std::cout << CYAN << "Send error headers for status: " << this->_event.getHttpStatus() << RESET << std::endl;
    write_headers(getErrorHeaders());
    if (EventStateHelper::is_error_state(this->_event)) return;

    this->_event.setHeaderSent(true);
}

void Response::write_default_error_page() {
    std::ostringstream error_page;
    std::string html_tag_init = "<html><body><h1>";
    std::string html_message = "Webserv Error: ";
    long error_status_code = this->_event.getHttpStatus();
    std::string html_tag_end =  "</h1></body></html>";

    error_page << html_tag_init << html_message << error_status_code << html_tag_end;

    std::cout << CYAN << "Response Page:\n" << error_page.str() << RESET << std::endl;
    if (send(_event.getClientFd(), error_page.str().c_str(), error_page.str().size(), 0) < 0) {
        std::cerr << RED << "Error while writing error page to client: " << strerror(errno) << RESET << std::endl;
    }

    std::cout << GREEN << "Successfully sent error page to client" << RESET << std::endl;
}

std::string Response::getErrorHeaders() {
    std::ostringstream headers;
    std::string error_header_init = "HTTP/1.1 ";
    long error_status_code = this->_event.getHttpStatus();
    std::string error_header_end =  "\r\nContent-Type: text/html\r\n\r\n";

    headers << error_header_init << error_status_code << error_header_end;
    return headers.str();
}

void Response::write_headers(const std::string &headers) {
    std::cout << CYAN << "Response Headers:\n" << headers << RESET << std::endl;

    if (send(_event.getClientFd(), headers.c_str(), headers.size(), 0) < 0) {
        std::cerr << RED << "Error while writing status header to client: " << strerror(errno) << RESET << std::endl;
        EventStateHelper::throw_error_state(this->_event, INTERNAL_SERVER_ERROR);
        return;
    }

    this->_event.setHeaderSent(true);
    std::cout << GREEN << "Successfully sent headers to client" << RESET << std::endl;
}
//
// Created by rmore on 10/05/2023.
//

#include "../../includes/io/Write.h"

Write::Write(Event &event): _event(event) {}

Write::~Write() {}


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

void Write::write_error_headers() {
    if (this->_event.isHeaderSent()) return;

    std::cout << CYAN << "Send error headers for status: " << this->_event.getHttpStatus() << RESET << std::endl;
    write_headers(getErrorHeaders());
    if (ErrorState::is_error_state(this->_event)) return;

    this->_event.setHeaderSent(true);
}

void Write::write_file_response_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    std::cout << MAGENTA << "Writing file response headers" << RESET << std::endl;

    write_headers(getFileHeaders(_event.getFilePath(), _event.getFileSize()));
    if (ErrorState::is_error_state(this->_event)) return;
}

void Write::write_headers(const std::string &headers) {
    std::cout << CYAN << "Response Headers:\n" << headers << RESET << std::endl;

    if (send(_event.getClientFd(), headers.c_str(), headers.size(), 0) < 0) {
        std::cerr << RED << "Error while writing status header to client: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, INTERNAL_SERVER_ERROR);
        return;
    }

    this->_event.setHeaderSent(true);
    std::cout << GREEN << "Successfully sent headers to client" << RESET << std::endl;
}

std::string Write::getErrorHeaders() {
    std::ostringstream headers;
    std::string error_header_init = "HTTP/1.1 ";
    long error_status_code = this->_event.getHttpStatus();
    std::string error_header_end =  "\r\nContent-Type: text/html\r\n\r\n";

    headers << error_header_init << error_status_code << error_header_end;
    return headers.str();
}


std::string Write::getFileHeaders(const std::string& file_path, size_t file_size) {
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
//
// Created by rmore on 10/05/2023.
//

#include "../../includes/model/Headers.h"

Headers::Headers():_mime_types() {}

Headers::~Headers() {}

std::string Headers::getFileHeaders(const std::string& file_path, size_t file_size) {
    char size_t_byte_buffer[25] = {};

    std::string extension = file_path.substr(file_path.find_last_of('.') + 1);

    if (snprintf(size_t_byte_buffer, 25, "%lu", file_size) < 0) {
        std::cerr << RED << "Error while formatting file size: " << strerror(errno) << RESET << std::endl;
        return "HTTP/1.1 200 Ok\r\nContent-Type: " + this->_mime_types.get_mime_type(extension) + "\r\n\r\n";
    }

    std::string content_length = "Content-Length: " + std::string(size_t_byte_buffer) + "\r\n";
    return "HTTP/1.1 200 Ok\r\n" + content_length + "Content-Type: " + this->_mime_types.get_mime_type(extension) + "\r\n\r\n";
}

std::string Headers::getErrorHeaders(event_http_status_enum_t status) {
    std::ostringstream headers;

    headers << "HTTP/1.1 " << status << "\r\nContent-Type: text/html\r\n\r\n";
    return headers.str();
}

std::string Headers::getRedirectionHeaders(const std::string &location) {
    std::ostringstream headers;

    headers << "HTTP/1.1 " << 301 << " Moved Permanently\r\nLocation: " << location << "\r\n\r\n";
    return headers.str();
}

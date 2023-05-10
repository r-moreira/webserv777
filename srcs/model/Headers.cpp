//
// Created by rmore on 10/05/2023.
//

#include "../../includes/model/Headers.h"

Headers::Headers():_mime_types() {}

Headers::~Headers() {}


std::string Headers::getErrorHeaders(event_http_status_enum_t status) {
    std::ostringstream headers;
    std::string error_header_init = "HTTP/1.1 ";
    long error_status_code = status;
    std::string error_header_end =  "\r\nContent-Type: text/html\r\n\r\n";

    headers << error_header_init << error_status_code << error_header_end;
    return headers.str();
}

std::string Headers::getFileHeaders(const std::string& file_path, size_t file_size) {
    char size_t_byte_buffer[25] = {};
    std::string headers = "HTTP/1.1 200 Ok\r\n";

    if (snprintf(size_t_byte_buffer, 25, "%lu", file_size) < 0) {
        std::cerr << RED << "Error while formatting file size: " << strerror(errno) << RESET << std::endl;
        //stop connection, display error page
    }
    std::string content_length = "Content-Length: " + std::string(size_t_byte_buffer) + "\r\n";

    std::string extension = file_path.substr(file_path.find_last_of('.') + 1);

    return "HTTP/1.1 200 Ok\r\n" + content_length + "Content-Type: " + this->_mime_types.get_mime_type(extension) +"\r\n\r\n";
}
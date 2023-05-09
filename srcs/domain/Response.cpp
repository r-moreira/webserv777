//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/Response.h"

Response::Response(Event &event): _event(event) {}

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

void Response::write_response_headers() {
    if (this->_event.getEventStatus() == Ended) return;

    send_headers(getFileHeaders(_event.getFilePath(), _event.getFileSize()));
    this->_event.setEventSubStatus(WritingResponseFile);
}

void Response::write_response_file() {
    if (this->_event.getEventStatus() == Ended) return;

    read_upload_file();

    write_upload_file();
}

void Response::write_error_headers() {
    send_headers(getErrorHeaders());
    this->_event.setEventSubStatus(WritingErrorPage);
}

void Response::write_error_page() {
    std::string error_500_page_html = "<html><body><h1>500 Internal Server Error</h1></body></html>";

    std::cout << CYAN << "Response Page:\n" << error_500_page_html << RESET << std::endl;

    if (send(_event.getClientFd(), error_500_page_html.c_str(), error_500_page_html.size(), 0) < 0) {
        std::cerr << RED << "Error while writing error page to client: " << strerror(errno) << RESET << std::endl;
    }

    _event.setEventStatus(Ended);
    std::cout << GREEN << "Successfully sent error page to client" << RESET << std::endl;
}

void Response::write_upload_file() {
    if (this->_event.getEventStatus() == Ended) return;

    long bytes_sent = send(_event.getClientFd() , _event.getFileReadChunkBuffer(), _event.getFileChunkReadBytes(), 0);
    if (bytes_sent < 0) {
        std::cerr << RED << "Error while writing to client: " << strerror(errno) << RESET << std::endl;
        //return error page, end connection
        _event.setEventStatus(Ended);
    }

    std::cout << YELLOW << "Transmitted Data Size " << bytes_sent << " Bytes." << RESET << std::endl;

    if (_event.getFileReadLeft() <= 0) {
        _event.setEventStatus(Ended);
        std::cout << GREEN << "File Transfer Complete." << RESET << std::endl;
    }
}

void Response::read_upload_file() {
    if (this->_event.getEventStatus() == Ended) return;

    size_t read_size;
    if (_event.getFileSize() > FILE_READ_CHUNK_SIZE) {
        read_size = _event.getFileReadLeft() > FILE_READ_CHUNK_SIZE ? FILE_READ_CHUNK_SIZE : _event.getFileReadLeft();
    } else {
        read_size = _event.getFileSize();
    }

    std::cout << YELLOW << "Read Data Size: " << read_size << RESET << std::endl;
    size_t chunk_bytes = fread((void *) this->_event.getFileReadChunkBuffer(), 1, read_size, _event.getFile());

    _event.setFileReadBytes(_event.getFileReadBytes() + chunk_bytes);
    std::cout << YELLOW << "Readed Data Size: " << chunk_bytes << RESET << std::endl;

    _event.setFileReadLeft(_event.getFileSize() - _event.getFileReadBytes());
    std::cout << YELLOW << "Read Left: " << _event.getFileReadLeft() << RESET << std::endl;

    this->_event.setFileChunkReadBytes(chunk_bytes);
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

std::string Response::getErrorHeaders() {
    std::string headers = "HTTP/1.1 500\r\n";
    headers += "Content-Type: text/html\r\n\r\n";
    return headers;
}

void Response::send_headers(const std::string &headers) {
    std::cout << CYAN << "Response Headers:\n" << headers << RESET << std::endl;

    if (send(_event.getClientFd(), headers.c_str(), headers.size(), 0) < 0) {
        std::cerr << RED << "Error while writing status header to client: " << strerror(errno) << RESET << std::endl;
        _event.setEventStatus(Ended);
        //return error page, end connection
    }

    std::cout << GREEN << "Successfully sent headers to client" << RESET << std::endl;
}


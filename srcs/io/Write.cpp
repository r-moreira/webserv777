//
// Created by rmore on 10/05/2023.
//

#include "../../includes/io/Write.h"

Write::Write(Event &event): _event(event) {}

Write::~Write() {}


void Write::write_requested_file() {
    if (ErrorState::is_error_state(this->_event)) return;

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

void Write::write_default_error_page() {
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

void Write::write_file_response_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    std::cout << MAGENTA << "Writing file response headers" << RESET << std::endl;

    write_headers(Headers::getFileHeaders(_event.getFilePath(), _event.getFileSize()));
    if (ErrorState::is_error_state(this->_event)) return;
}

void Write::write_error_headers() {
    if (this->_event.isHeaderSent()) return;

    std::cout << CYAN << "Send error headers for status: " << this->_event.getHttpStatus() << RESET << std::endl;
    write_headers(Headers::getErrorHeaders(this->_event.getHttpStatus()));
    if (ErrorState::is_error_state(this->_event)) return;

    this->_event.setHeaderSent(true);
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
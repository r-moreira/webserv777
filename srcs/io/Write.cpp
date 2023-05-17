//
// Created by rmore on 10/05/2023.
//

#include "../../includes/io/Write.h"

Write::Write(Event &event): _event(event), _headers() {}

Write::~Write() {}


void Write::write_requested_file() {
    if (ErrorState::is_error_state(this->_event)) return;

    long bytes_sent = send(_event.getClientFd() , _event.getFileReadChunkBuffer(), _event.getFileChunkReadBytes(), 0);
    if (bytes_sent < 0) {
        std::cerr << RED << "Error while writing to client: " << strerror(errno) << RESET << std::endl;
        _event.setEventStatus(Event::Ended);
        return;
    }

    std::cout << YELLOW << "Transmitted Data Size " << bytes_sent << " Bytes." << RESET << std::endl;

    if (_event.getFileReadLeft() <= 0) {
        _event.setEventStatus(Event::Ended);
        std::cout << GREEN << "File Transfer Complete." << RESET << std::endl;
    }
}

void Write::write_upload_file() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << CYAN << "Writing uploaded file to disk" << RESET << std::endl;

    size_t bytes_written = fwrite(this->_event.getUploadFileChunkBuffer(), 1, _event.getFileChunkReadBytes(), this->_event.getFile());
    if (bytes_written != _event.getFileChunkReadBytes()) {
        std::cerr << RED << "Error while writing file to disk: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::INTERNAL_SERVER_ERROR);
        return;
    }

    std::cout << YELLOW << "Written Data Size " << bytes_written << " Bytes." << RESET << std::endl;


    if (_event.getFileReadLeft() <= 0) {
        _event.setEventStatus(Event::Ended);
        std::cout << GREEN << "File Transfer Complete." << RESET << std::endl;
    }
}

void Write::write_remaining_read_buffer_to_file() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isRemainingReadBytesWritedToFile()) return;

    std::cout << CYAN << "Writing remaining read buffer to file" << RESET << std::endl;

    size_t remaining_read_request_bytes = this->_event.getRemainingReadBuffer().size();
    size_t remaining_file_bytes = this->_event.getRemainingFileUploadBytes();
    size_t read_size = remaining_file_bytes < remaining_read_request_bytes ? remaining_file_bytes : remaining_read_request_bytes;


    size_t bytes_written = fwrite(this->_event.getRemainingReadBuffer().c_str(), 1, read_size, this->_event.getFile());
    if (bytes_written != read_size) {
        std::cerr << RED << "Error while writing file to disk: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::INTERNAL_SERVER_ERROR);
        return;
    }

    this->_event.setRemainingFileUploadBytes(remaining_file_bytes - read_size);

    if (this->_event.getRemainingFileUploadBytes() == 0) {
        std::cout << GREEN << "File Upload Complete" << RESET << std::endl;
        this->_event.setEventStatus(Event::Ended);
        return;
    }

    this->_event.setRemainingReadBytesWritedToFile(true);
}

void Write::write_default_error_page() {
    std::string default_error_page = Pages::get_default_error_page(this->_event.getHttpStatus());

    std::cout << CYAN << "Response Page:\n" << default_error_page << RESET << std::endl;

    if (send(_event.getClientFd(), default_error_page.c_str(), default_error_page.size(), 0) < 0) {
        std::cerr << RED << "Error while writing error page to client: " << strerror(errno) << RESET << std::endl;
    }

    std::cout << GREEN << "Successfully sent error page to client" << RESET << std::endl;
}

void Write::write_file_response_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    std::cout << MAGENTA << "Writing file response headers" << RESET << std::endl;

    write_headers(this->_headers.getFileHeaders(_event.getFilePath(), _event.getFileSize()));
    if (ErrorState::is_error_state(this->_event)) return;
}

void Write::write_error_headers() {
    if (this->_event.isHeaderSent()) return;

    std::cout << CYAN << "Send error headers for status: " << this->_event.getHttpStatus() << RESET << std::endl;

    write_headers(this->_headers.getErrorHeaders(this->_event.getHttpStatus()));
    if (ErrorState::is_error_state(this->_event)) return;

    this->_event.setHeaderSent(true);
}
void Write::write_redirection_headers() {
    if (this->_event.isForcedRedirect()) {
        std::cout << CYAN << "Send forced redirection headers:" << RESET << std::endl;

        write_headers(this->_headers.getRedirectionHeaders(this->_event.getForcedRedirectLocation()));
        if (ErrorState::is_error_state(this->_event)) return;
        this->_event.setHeaderSent(true);
        return;
    }

    std::cout << CYAN << "Send redirection headers:" << RESET << std::endl;
    write_headers(this->_headers.getRedirectionHeaders(this->_event.getLocation().getRedirectLocation()));

    if (ErrorState::is_error_state(this->_event)) return;
    this->_event.setHeaderSent(true);
}


void Write::write_created_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    std::cout << CYAN << "Send created headers:" << RESET << std::endl;

    write_headers(this->_headers.getCreatedHeaders(this->_event.getFilePath()));
    if (ErrorState::is_error_state(this->_event)) return;
    this->_event.setHeaderSent(true);
}


void Write::write_headers(const std::string &headers) {
    std::cout << CYAN << "Response Headers:\n" << headers << RESET << std::endl;

    if (send(_event.getClientFd(), headers.c_str(), headers.size(), 0) < 0) {
        std::cerr << RED << "Error while writing status header to client: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::INTERNAL_SERVER_ERROR);
        return;
    }

    this->_event.setHeaderSent(true);
    std::cout << GREEN << "Successfully sent headers to client" << RESET << std::endl;
}

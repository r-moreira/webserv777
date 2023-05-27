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
        _event.setEventStatus(Event::Status::Ended);
        return;
    }

    std::cout << YELLOW << "Transmitted Data Size " << bytes_sent << " Bytes." << RESET << std::endl;

    if (_event.getFileReadLeft() <= 0) {
        _event.setEventStatus(Event::Status::Ended);
        std::cout << GREEN << "File Transfer Complete." << RESET << std::endl;
    }
}

void Write::write_upload_file() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << CYAN << "Writing uploaded file to disk" << RESET << std::endl;

    size_t bytes_written = fwrite(this->_event.getContentChunkBuffer(), 1, _event.getFileChunkReadBytes(), this->_event.getFile());
    if (bytes_written != _event.getFileChunkReadBytes()) {
        std::cerr << RED << "Error while writing file to disk: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    std::cout << YELLOW << "Written Data Size " << bytes_written << " Bytes." << RESET << std::endl;


    if (_event.getFileReadLeft() <= 0) {
        _event.setEventStatus(Event::Status::Ended);
        std::cout << GREEN << "File Transfer Complete." << RESET << std::endl;
    }
}

void Write::write_remaining_read_buffer_to_file() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isRemainingReadBytesWrited()) return;

    std::cout << CYAN << "Writing remaining read buffer to file" << RESET << std::endl;

    size_t remaining_read_request_bytes = this->_event.getRemainingReadBuffer().size();
    size_t remaining_file_bytes = this->_event.getRemainingFileBytes();
    size_t write_size = remaining_file_bytes < remaining_read_request_bytes ? remaining_file_bytes : remaining_read_request_bytes;


    size_t bytes_written = fwrite(this->_event.getRemainingReadBuffer().c_str(), 1, write_size, this->_event.getFile());
    if (bytes_written != write_size) {
        std::cerr << RED << "Error while writing file to disk: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    size_t file_read_left = remaining_file_bytes - bytes_written;
    this->_event.setRemainingFileBytes(file_read_left);
    this->_event.setFileReadLeft(file_read_left);

    if (this->_event.getRemainingFileBytes() == 0) {
        std::cout << GREEN << "File Upload Complete" << RESET << std::endl;
        this->_event.setEventStatus(Event::Status::Ended);
        return;
    }

    this->_event.setRemainingReadBytesWrited(true);
}

void Write::write_body_to_cgi() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << CYAN << "Writing remaining read buffer to cgi" << RESET << std::endl;

    size_t remaining_read_request_bytes = this->_event.getRemainingReadBuffer().size();
    size_t remaining_cgi_bytes = this->_event.getRemainingFileBytes();
    size_t write_size = remaining_cgi_bytes < remaining_read_request_bytes ? remaining_cgi_bytes : remaining_read_request_bytes;

    int _pipeFd[2];
    if (pipe(_pipeFd) < 0) {
        std::cout << RED << "Error: unable to Pipe" << RESET << '\n';
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
    }
    this->_event.setServerCgiFdOut(_pipeFd[0]);
    this->_event.setServerCgiFdIn(_pipeFd[1]);

    size_t bytes_written = 0;
    bytes_written = write(this->_event.getServerCgiFdIn(), this->_event.getRemainingReadBuffer().c_str(), write_size);
    if (bytes_written != write_size) {
        std::cerr << RED << "Error while writing to CGI STDIN: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
    }

    size_t file_read_left = remaining_cgi_bytes - bytes_written;
    this->_event.setRemainingFileBytes(file_read_left);
    this->_event.setFileReadLeft(file_read_left);

    if (this->_event.getRemainingFileBytes() == 0) {
        std::cout << GREEN << "\nCGI STDIN Write Complete" << RESET << std::endl;
        return;
    }

    char buffer[BODY_READ_BUFFER_SIZE] = {};

    std::cout << YELLOW << "Reading body content and sending it to CGI STDIN, remaining bytes: "
        << this->_event.getRemainingFileBytes() << RESET << std::endl;

    size_t read_size = this->_event.getRemainingFileBytes() < BODY_READ_BUFFER_SIZE ? this->_event.getRemainingFileBytes() : BODY_READ_BUFFER_SIZE;

    long read_bytes = 0;
    while ((read_bytes = read(this->_event.getClientFd(), buffer, read_size)) > 0) {
        if (read_bytes < 0) {
            std::cerr << RED << "Error while reading client body: " << strerror(errno) << RESET << std::endl;
            _event.setEventStatus(Event::Status::Ended);
            return;
        }

        std::cout << YELLOW << "Read " << read_bytes << " bytes from client body" << RESET << std::endl;

        bytes_written = write(this->_event.getServerCgiFdIn(), buffer, read_bytes);
        if (bytes_written != read_bytes) {
            std::cerr << RED << "Error while writing to CGI STDIN: " << strerror(errno) << RESET << std::endl;
            ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
            break;
        }
        std::cout << YELLOW << "Transmitted " << bytes_written << " byte to CGI STDIN" << RESET << std::endl;

        size_t _file_read_left = this->_event.getRemainingFileBytes() - bytes_written;
        this->_event.setRemainingFileBytes(_file_read_left);
        this->_event.setFileReadLeft(_file_read_left);

        if (_file_read_left == 0) {
            std::cout << GREEN << "\nCGI STDIN Write Complete!" << RESET << std::endl;
            break;
        }

    }
}

void Write::write_auto_index_page(const std::string& auto_index_page) {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << CYAN << "Writing auto index page" << RESET << std::endl;

    if (send(_event.getClientFd(), auto_index_page.c_str(), auto_index_page.size(), 0) < 0) {
        std::cerr << RED << "Error while writing auto index page to client: " << strerror(errno) << RESET << std::endl;
    }

    _event.setEventStatus(Event::Status::Ended);
}

void Write::write_default_error_page() {
    std::string default_error_page = Pages::get_default_error_page(this->_event.getHttpStatus());

    std::cout << CYAN << "Response Page:\n" << default_error_page << RESET << std::endl;

    if (send(_event.getClientFd(), default_error_page.c_str(), default_error_page.size(), 0) < 0) {
        std::cerr << RED << "Error while writing error page to client: " << strerror(errno) << RESET << std::endl;
    }

    std::cout << GREEN << "Successfully sent error page to client" << RESET << std::endl;
}

void Write::write_cgi_content() {
    if (ErrorState::is_error_state(this->_event)) return;

    char buffer[CGI_BUFFER_SIZE] = {};

    long read_bytes = 0;
    while ((read_bytes = read(this->_event.getCgiFdOut(), buffer, CGI_BUFFER_SIZE)) > 0) {
        if (read_bytes < 0) {
            std::cerr << RED << "Error while reading CGI output: " << strerror(errno) << RESET << std::endl;
            _event.setEventStatus(Event::Status::Ended);
            return;
        }

        std::cout << YELLOW << "Read " << read_bytes << " bytes from CGI" << RESET << std::endl;
        //std::cout << MAGENTA << "CGI Buffer: |" << buffer << "|" << RESET << std::endl;

        long bytes_sent = send(_event.getClientFd(), buffer, read_bytes, 0);

        if (bytes_sent < 0) {
            std::cerr << RED << "Error while writing CGI content to client: " << strerror(errno) << RESET << std::endl;
            _event.setEventStatus(Event::Status::Ended);
            return;
        }
        std::cout << YELLOW << "Transmitted " << bytes_sent << " byte from CGI" << RESET << std::endl;
    }

    std::cout << GREEN << "CGI Content Write Complete" << RESET << std::endl;
    _event.setEventStatus(Event::Status::Ended);
}

void Write::write_file_response_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    std::cout << MAGENTA << "Writing file response headers" << RESET << std::endl;

    _write_headers(this->_headers.getFileHeaders(_event.getFilePath(), _event.getFileSize()));
    if (ErrorState::is_error_state(this->_event)) return;
}

void Write::write_error_headers() {
    if (this->_event.isHeaderSent()) return;

    std::cout << CYAN << "Send error headers for status: " << this->_event.getHttpStatus() << RESET << std::endl;

    _write_headers(this->_headers.getErrorHeaders(this->_event.getHttpStatus()));
    if (ErrorState::is_error_state(this->_event)) return;

    this->_event.setHeaderSent(true);
}

void Write::write_redirection_headers() {
    if (this->_event.isForcedRedirect()) {
        std::cout << CYAN << "Send forced redirection headers:" << RESET << std::endl;

        _write_headers(this->_headers.getRedirectionHeaders(this->_event.getForcedRedirectLocation()));
        if (ErrorState::is_error_state(this->_event)) return;
        this->_event.setHeaderSent(true);
        return;
    }

    std::cout << CYAN << "Send redirection headers:" << RESET << std::endl;
    _write_headers(this->_headers.getRedirectionHeaders(this->_event.getLocation().getRedirectLocation()));

    if (ErrorState::is_error_state(this->_event)) return;
    this->_event.setHeaderSent(true);
}

void Write::write_auto_index_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    std::cout << CYAN << "Send auto index headers:" << RESET << std::endl;

    _write_headers(this->_headers.getAutoIndexHeaders());
    if (ErrorState::is_error_state(this->_event)) return;
    this->_event.setHeaderSent(true);
}

void Write::write_created_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    std::cout << CYAN << "Send created headers:" << RESET << std::endl;

    _write_headers(this->_headers.getCreatedHeaders(this->_event.getFilePath()));
    if (ErrorState::is_error_state(this->_event)) return;
    this->_event.setHeaderSent(true);
}


void Write::write_no_content_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    std::cout << CYAN << "Send no content headers:" << RESET << std::endl;

    _write_headers(this->_headers.getNoContentHeaders(this->_event.getFilePath()));
    if (ErrorState::is_error_state(this->_event)) return;
    this->_event.setHeaderSent(true);
}

void Write::write_cgi_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    std::cout << CYAN << "Send CGI headers:" << RESET << std::endl;
    _write_headers(this->_headers.getCGIHeaders(this->_event.getHttpStatus()));
    if (ErrorState::is_error_state(this->_event)) return;
    _event.setHeaderSent(true);
}


void Write::_write_headers(const std::string &headers) {
    std::cout << CYAN << "Response Headers:\n" << headers << RESET << std::endl;

    if (send(_event.getClientFd(), headers.c_str(), headers.size(), 0) < 0) {
        std::cerr << RED << "Error while writing status header to client: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    this->_event.setHeaderSent(true);
    std::cout << GREEN << "Successfully sent headers to client" << RESET << std::endl;
}


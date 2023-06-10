//
// Created by rmore on 10/05/2023.
//

#include "../../includes/io/Write.hpp"

Write::Write(Event &event): _event(event), _headers() {}

Write::~Write() {}


void Write::write_requested_file() {
    if (ErrorState::is_error_state(this->_event)) return;

    long bytes_sent = send(_event.getClientFd() , _event.getFileReadChunkBuffer(), _event.getFileChunkReadBytes(), 0);
    if (bytes_sent < 0) {
        Logger::error("Error while writing to client: " + std::string(strerror(errno)));
        _event.setEventStatus(Event::Status::Ended);
        return;
    }

    Logger::trace("Transmitted Data Size: " + ITOSTR(bytes_sent) + " Bytes.");

    if (_event.getFileReadLeft() <= 0) {
        _event.setEventStatus(Event::Status::Ended);
        Logger::debug("File Transfer Complete.");
    }
}

void Write::write_upload_file() {
    if (ErrorState::is_error_state(this->_event)) return;

    Logger::trace("Writing uploaded file to disk");

    size_t bytes_written = fwrite(this->_event.getContentChunkBuffer(), 1, _event.getFileChunkReadBytes(), this->_event.getFile());
    if (bytes_written != _event.getFileChunkReadBytes()) {
        Logger::error("Error while writing file to disk: " + std::string(strerror(errno)));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    Logger::trace("Written Data Size " + ITOSTR(bytes_written) + " Bytes.");

    if (_event.getFileReadLeft() <= 0) {
        _event.setEventStatus(Event::Status::Ended);
        Logger::debug("File Transfer Complete.");
    }
}

void Write::write_remaining_read_buffer_to_file() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isRemainingReadBytesWrited()) return;

    Logger::trace("Writing remaining read buffer to file");

    size_t remaining_read_request_bytes = this->_event.getRemainingReadBuffer().size();
    size_t remaining_file_bytes = this->_event.getRemainingFileBytes();
    size_t write_size = remaining_file_bytes < remaining_read_request_bytes ? remaining_file_bytes : remaining_read_request_bytes;


    size_t bytes_written = fwrite(this->_event.getRemainingReadBuffer().c_str(), 1, write_size, this->_event.getFile());
    if (bytes_written != write_size) {
        Logger::error("Error while writing file to disk: " + std::string(strerror(errno)));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    size_t file_read_left = remaining_file_bytes - bytes_written;
    this->_event.setRemainingFileBytes(file_read_left);
    this->_event.setFileReadLeft(file_read_left);

    if (this->_event.getRemainingFileBytes() == 0) {
        Logger::debug("File Upload Complete");
        this->_event.setEventStatus(Event::Status::Ended);
        return;
    }

    this->_event.setRemainingReadBytesWrited(true);
}

void Write::write_body_to_cgi() {
    if (ErrorState::is_error_state(this->_event)) return;

    Logger::trace("Writing body to cgi");

    size_t remaining_read_request_bytes = this->_event.getRemainingReadBuffer().size();
    size_t remaining_cgi_bytes = this->_event.getRemainingFileBytes();
    size_t write_size = remaining_cgi_bytes < remaining_read_request_bytes ? remaining_cgi_bytes : remaining_read_request_bytes;

    int _pipeFd[2];
    if (pipe(_pipeFd) < 0) {
        Logger::error("Error while piping: " + std::string(strerror(errno)));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
    }
    this->_event.setServerCgiFdOut(_pipeFd[0]);
    this->_event.setServerCgiFdIn(_pipeFd[1]);

    size_t bytes_written = 0;
    bytes_written = write(this->_event.getServerCgiFdIn(), this->_event.getRemainingReadBuffer().c_str(), write_size);
    if (bytes_written != write_size) {
        Logger::error("Error while writing to CGI STDIN: " + std::string(strerror(errno)));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
    }

    size_t file_read_left = remaining_cgi_bytes - bytes_written;
    this->_event.setRemainingFileBytes(file_read_left);
    this->_event.setFileReadLeft(file_read_left);

    if (this->_event.getRemainingFileBytes() == 0) {
        Logger::trace("CGI STDIN Write Complete");
        return;
    }

    char buffer[BODY_READ_BUFFER_SIZE] = {};

    Logger::trace("Reading body content and sending it to CGI STDIN, remaining bytes: " + ITOSTR(this->_event.getRemainingFileBytes()) + " Bytes.");

    size_t read_size = this->_event.getRemainingFileBytes() < BODY_READ_BUFFER_SIZE ? this->_event.getRemainingFileBytes() : BODY_READ_BUFFER_SIZE;

    long read_bytes = 0;
    while ((read_bytes = read(this->_event.getClientFd(), buffer, read_size)) > 0) {
        if (read_bytes < 0) {
            Logger::error("Error while reading client body: " + std::string(strerror(errno)));
            _event.setEventStatus(Event::Status::Ended);
            return;
        }

        Logger::trace("Read " + ITOSTR(read_bytes) + " Bytes from client body");

        bytes_written = write(this->_event.getServerCgiFdIn(), buffer, read_bytes);
        if ((long)bytes_written != read_bytes) {
            std::cerr << RED << "Error while writing to CGI STDIN: " << strerror(errno) << RESET << std::endl;
            ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
            break;
        }

        Logger::trace("Transmitted " + ITOSTR(bytes_written) + " byte to CGI STDIN");

        size_t _file_read_left = this->_event.getRemainingFileBytes() - bytes_written;
        this->_event.setRemainingFileBytes(_file_read_left);
        this->_event.setFileReadLeft(_file_read_left);

        if (_file_read_left == 0) {
            Logger::trace("CGI STDIN Write Complete");
            break;
        }

    }
}

void Write::write_auto_index_page(const std::string& auto_index_page) {
    if (ErrorState::is_error_state(this->_event)) return;

    Logger::trace("Writing auto index page");

    if (send(_event.getClientFd(), auto_index_page.c_str(), auto_index_page.size(), 0) < 0) {
        Logger::error("Error while writing auto index page to client: " + std::string(strerror(errno)));
    }

    _event.setEventStatus(Event::Status::Ended);
}

void Write::write_default_error_page() {
    std::string default_error_page = Pages::get_default_error_page(this->_event.getHttpStatus());

    Logger::trace("Writting default error response Page:\n" + default_error_page);

    if (send(_event.getClientFd(), default_error_page.c_str(), default_error_page.size(), 0) < 0) {
        Logger::error("Error while writing default error page to client: " + std::string(strerror(errno)));
     }

    Logger::debug("Successfully sent error page to client");
}

void Write::write_cgi_content() {
    if (ErrorState::is_error_state(this->_event)) return;

    char buffer[CGI_BUFFER_SIZE] = {};

    long read_bytes = 0;
    while ((read_bytes = read(this->_event.getCgiFdOut(), buffer, CGI_BUFFER_SIZE)) > 0) {
        if (read_bytes < 0) {
            Logger::error("Error while reading CGI output: " + std::string(strerror(errno)));
            _event.setEventStatus(Event::Status::Ended);
            return;
        }

        Logger::trace("Read " + ITOSTR(read_bytes) + " Bytes from CGI");

        long bytes_sent = send(_event.getClientFd(), buffer, read_bytes, 0);

        if (bytes_sent < 0) {
            Logger::error("Error while writing CGI content to client: " + std::string(strerror(errno)));
            _event.setEventStatus(Event::Status::Ended);
            return;
        }

        Logger::trace("Transmitted " + ITOSTR(bytes_sent) + " byte from CGI");
    }

    Logger::debug("Successfully sent CGI content to client");
    _event.setEventStatus(Event::Status::Ended);
}

void Write::write_file_response_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    Logger::trace("Writing file response headers");

    _write_headers(this->_headers.getFileHeaders(_event.getFilePath(), _event.getFileSize()));
    if (ErrorState::is_error_state(this->_event)) return;
}

void Write::write_error_headers() {
    if (this->_event.isHeaderSent()) return;

    Logger::trace("Sendding error headers for status: " + ITOSTR(this->_event.getHttpStatus()));

    _write_headers(this->_headers.getErrorHeaders(this->_event.getHttpStatus()));
    if (ErrorState::is_error_state(this->_event)) return;

    this->_event.setHeaderSent(true);
}

void Write::write_redirection_headers() {
    if (this->_event.isForcedRedirect()) {
        Logger::trace("Sending forced redirection headers");

        _write_headers(this->_headers.getRedirectionHeaders(this->_event.getForcedRedirectLocation()));
        if (ErrorState::is_error_state(this->_event)) return;
        this->_event.setHeaderSent(true);
        return;
    }

    _write_headers(this->_headers.getRedirectionHeaders(this->_event.getLocation().getRedirectLocation()));

    if (ErrorState::is_error_state(this->_event)) return;
    this->_event.setHeaderSent(true);
}

void Write::write_auto_index_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    Logger::trace("Sending auto index headers");

    _write_headers(this->_headers.getAutoIndexHeaders());
    if (ErrorState::is_error_state(this->_event)) return;
    this->_event.setHeaderSent(true);
}

void Write::write_created_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    Logger::trace("Sending created headers");

    _write_headers(this->_headers.getCreatedHeaders(this->_event.getFilePath()));
    if (ErrorState::is_error_state(this->_event)) return;
    this->_event.setHeaderSent(true);
}


void Write::write_no_content_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    Logger::trace("Sending no content headers");

    _write_headers(this->_headers.getNoContentHeaders(this->_event.getFilePath()));
    if (ErrorState::is_error_state(this->_event)) return;
    this->_event.setHeaderSent(true);
}

void Write::write_cgi_headers() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isHeaderSent()) return;

    Logger::trace("Sending CGI headers");

    _write_headers(this->_headers.getCGIHeaders(this->_event.getHttpStatus()));
    if (ErrorState::is_error_state(this->_event)) return;
    _event.setHeaderSent(true);
}


void Write::_write_headers(const std::string &headers) {
    Logger::debug("Response Headers:\n" + headers);

    if (send(_event.getClientFd(), headers.c_str(), headers.size(), 0) < 0) {
        Logger::error("Error while writing headers to client: " + std::string(strerror(errno)));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    this->_event.setHeaderSent(true);
    Logger::debug("Successfully sent headers to client");
}


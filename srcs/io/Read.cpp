//
// Created by rmore on 10/05/2023.
//

#include "../../includes/io/Read.h"

Read::Read(Event &event): _event(event) {}

Read::~Read() {}

void Read::read_request() {
    if (ErrorState::is_error_state(this->_event)) return;

    char buffer[REQUEST_READ_BUFFER_SIZE] = {'\0'};

    long bytes_read = read(this->_event.getClientFd(), buffer, REQUEST_READ_BUFFER_SIZE);

    if (bytes_read == -1) {

        Logger::error("Error while reading from client: " + std::string(strerror(errno)));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    } else if (bytes_read == 0) {
        Logger::warning("Client disconnected");
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::CLIENT_CLOSED_REQUEST);
        return;
    }

    this->_event.setRequestReadBytes(this->_event.getRequestReadBytes() + bytes_read);
    std::string read_buffer = this->_event.getRequestReadBuffer();
    this->_event.setRequestReadBuffer(read_buffer.append(buffer, bytes_read));

    Logger::trace("Read " + ITOSTR(bytes_read) + " bytes from client");
    Logger::trace("HTTP Request:\n" + this->_event.getRequestReadBuffer());
}

void Read::read_body_content() {
    if (ErrorState::is_error_state(this->_event)) return;

    Logger::trace("Reading body content from client");
    std::memset((void *) this->_event.getContentChunkBuffer(), '\0', BODY_READ_BUFFER_SIZE);

    size_t read_size;
    if (this->_event.getFileReadLeft() > BODY_READ_BUFFER_SIZE) {
        read_size = BODY_READ_BUFFER_SIZE;
    } else {
        read_size = this->_event.getFileReadLeft();
    }

    long chunk_bytes = read(this->_event.getClientFd(), (void *) this->_event.getContentChunkBuffer(), read_size);

    if (chunk_bytes == -1) {
        Logger::error("Error while reading from client: " + std::string(strerror(errno)));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    } else if (chunk_bytes == 0) {
        Logger::warning("Client disconnected");
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::CLIENT_CLOSED_REQUEST);
        return;
    }

    this->_event.setFileReadBytes(this->_event.getFileReadBytes() + chunk_bytes);
    this->_event.setFileReadLeft(this->_event.getRemainingFileBytes() - this->_event.getFileReadBytes());
    this->_event.setFileChunkReadBytes(chunk_bytes);

    Logger::trace("Read " + ITOSTR(chunk_bytes) + " bytes from client");
    Logger::trace("Read Left: " + ITOSTR(this->_event.getFileReadLeft()));
}

void Read::read_file() {
    if (ErrorState::is_error_state(this->_event)) return;

    Logger::trace("Reading request file: " + this->_event.getFilePath());

    std::memset((void *) this->_event.getFileReadChunkBuffer(), '\0', FILE_READ_CHUNK_SIZE);

    size_t read_size;
    if (this->_event.getFileSize() > FILE_READ_CHUNK_SIZE) {
        read_size = this->_event.getFileReadLeft() > FILE_READ_CHUNK_SIZE ? FILE_READ_CHUNK_SIZE : _event.getFileReadLeft();
    } else {
        read_size = this->_event.getFileSize();
    }

    Logger::trace("Read Size: " + ITOSTR(read_size));
    size_t chunk_bytes = fread((void *) this->_event.getFileReadChunkBuffer(), 1, read_size, _event.getFile());

    if (ferror(_event.getFile())) {
        Logger::error("Error while reading file: " + std::string(strerror(errno)));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    this->_event.setFileReadBytes(this->_event.getFileReadBytes() + chunk_bytes);
    this->_event.setFileReadLeft(this->_event.getFileSize() - this->_event.getFileReadBytes());
    this->_event.setFileChunkReadBytes(chunk_bytes);

    Logger::trace("Read Data Size " + ITOSTR(chunk_bytes) + " bytes from file");
    Logger::trace("Read Left: " + ITOSTR(this->_event.getFileReadLeft()));
}

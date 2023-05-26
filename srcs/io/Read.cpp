//
// Created by rmore on 10/05/2023.
//

#include "../../includes/io/Read.h"

Read::Read(Event &event): _event(event) {}

Read::~Read() {}

void Read::read_request() {
    if (ErrorState::is_error_state(this->_event)) return;

    char buffer[REQUEST_READ_BUFFER_SIZE] = {};

    long bytes_read = read(this->_event.getClientFd(), buffer, REQUEST_READ_BUFFER_SIZE);

    if (bytes_read == -1) {
        std::cerr << RED << "Error while reading from client: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    } else if (bytes_read == 0) {
        std::cout << YELLOW << "Client disconnected" << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::CLIENT_CLOSED_REQUEST);
        return;
    }

    this->_event.setRequestReadBytes(this->_event.getRequestReadBytes() + bytes_read);
    std::string read_buffer = this->_event.getRequestReadBuffer();
    this->_event.setRequestReadBuffer(read_buffer.append(buffer, bytes_read));

    std::cout << YELLOW << "Read " << this->_event.getRequestReadBytes() << " bytes from client" << RESET << std::endl;
    std::cout << GREEN << "HTTP Request:\n" << this->_event.getRequestReadBuffer() << RESET << std::endl;
}

void Read::read_upload_file() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << MAGENTA << "Reading uploaded file from client" << RESET << std::endl;
    std::memset((void *) this->_event.getUploadFileChunkBuffer(), '\0', UPLOAD_BUFFER_SIZE);

    size_t read_size;
    if (this->_event.getFileReadLeft() > UPLOAD_BUFFER_SIZE) {
        read_size =UPLOAD_BUFFER_SIZE;
    } else {
        read_size = this->_event.getFileReadLeft();
    }

    long chunk_bytes = read(this->_event.getClientFd(), (void *) this->_event.getUploadFileChunkBuffer(), read_size);

    if (chunk_bytes == -1) {
        std::cerr << RED << "Error while reading from client: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    } else if (chunk_bytes == 0) {
        std::cout << YELLOW << "Client disconnected" << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::CLIENT_CLOSED_REQUEST);
        return;
    }

    this->_event.setFileReadBytes(this->_event.getFileReadBytes() + chunk_bytes);
    std::cout << YELLOW << "Readed Data Size: " << chunk_bytes << RESET << std::endl;

    this->_event.setFileReadLeft(this->_event.getRemainingFileBytes() - this->_event.getFileReadBytes());
    std::cout << YELLOW << "Read Left: " << this->_event.getFileReadLeft() << RESET << std::endl;

    this->_event.setFileChunkReadBytes(chunk_bytes);
}

void Read::read_file() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << MAGENTA << "Reading request file: " << this->_event.getFilePath() << RESET << std::endl;
    std::memset((void *) this->_event.getFileReadChunkBuffer(), '\0', FILE_READ_CHUNK_SIZE);

    size_t read_size;
    if (this->_event.getFileSize() > FILE_READ_CHUNK_SIZE) {
        read_size = this->_event.getFileReadLeft() > FILE_READ_CHUNK_SIZE ? FILE_READ_CHUNK_SIZE : _event.getFileReadLeft();
    } else {
        read_size = this->_event.getFileSize();
    }

    std::cout << YELLOW << "Read Data Size: " << read_size << RESET << std::endl;
    size_t chunk_bytes = fread((void *) this->_event.getFileReadChunkBuffer(), 1, read_size, _event.getFile());

    if (ferror(_event.getFile())) {
        std::cerr << RED << "Error while reading file: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    this->_event.setFileReadBytes(this->_event.getFileReadBytes() + chunk_bytes);
    std::cout << YELLOW << "Readed Data Size: " << chunk_bytes << RESET << std::endl;

    this->_event.setFileReadLeft(this->_event.getFileSize() - this->_event.getFileReadBytes());
    std::cout << YELLOW << "Read Left: " << this->_event.getFileReadLeft() << RESET << std::endl;

    this->_event.setFileChunkReadBytes(chunk_bytes);
}

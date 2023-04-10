//
// Created by rmore on 09/04/2023.
//

#include "../../includes/domain/Event.h"

Event::Event(int client_fd) {
    this->_client_fd = client_fd;

    this->_request = Request();
    this->_request_read_buffer = "";
    this->_request_read_bytes = 0;

    this->_file = NULL;
    this->_file_path = "";
    this->_file_read_bytes = 0;
    this->_file_read_left = 0;
    this->_file_chunk_read_bytes = 0;
    this->_file_size = 0;
    this->_file_read_chunk_buffer[0] = '\0';

    this->_event_status = Reading;
    this->_event_sub_status = ReadingRequest;
}

Event::~Event() {
    if (_file != NULL) {
        fclose(_file);
    }
    close(this->getClientFd());
}


int Event::getClientFd() const {
    return _client_fd;
}

void Event::setClientFd(int clientFd) {
    _client_fd = clientFd;
}

const Request &Event::getRequest() const {
    return _request;
}

void Event::setRequest(const Request &request) {
    _request = request;
}

const std::string &Event::getRequestReadBuffer() const {
    return _request_read_buffer;
}

void Event::setRequestReadBuffer(const std::string &requestReadBuffer) {
    _request_read_buffer = requestReadBuffer;
}

const std::string &Event::getFilePath() const {
    return _file_path;
}

void Event::setFilePath(const std::string &filePath) {
    _file_path = filePath;
}

size_t Event::getRequestReadBytes() const {
    return _request_read_bytes;
}

void Event::setRequestReadBytes(size_t requestReadBytes) {
    _request_read_bytes = requestReadBytes;
}

FILE *Event::getFile() const {
    return _file;
}

void Event::setFile(FILE *file) {
    _file = file;
}

size_t Event::getFileReadBytes() const {
    return _file_read_bytes;
}

void Event::setFileReadBytes(size_t fileReadBytes) {
    _file_read_bytes = fileReadBytes;
}

size_t Event::getFileReadLeft() const {
    return _file_read_left;
}

void Event::setFileReadLeft(size_t fileReadLeft) {
    _file_read_left = fileReadLeft;
}

size_t Event::getFileChunkReadBytes() const {
    return _file_chunk_read_bytes;
}

void Event::setFileChunkReadBytes(size_t fileChunkReadBytes) {
    _file_chunk_read_bytes = fileChunkReadBytes;
}

size_t Event::getFileSize() const {
    return _file_size;
}

void Event::setFileSize(size_t fileSize) {
    _file_size = fileSize;
}

const char *Event::getFileReadChunkBuffer() const {
    return _file_read_chunk_buffer;
}

event_status_t Event::getEventStatus() const {
    return _event_status;
}

void Event::setEventStatus(event_status_t eventStatus) {
    _event_status = eventStatus;
}

event_sub_status_t Event::getEventSubStatus() const {
    return _event_sub_status;
}

void Event::setEventSubStatus(event_sub_status_t eventSubStatus) {
    _event_sub_status = eventSubStatus;
}


//
// Created by rmore on 09/04/2023.
//

#include "../../includes/domain/Event.h"

Event::Event(int server_fd, int client_fd) {
    this->_client_fd = client_fd;
    this->_server_fd = server_fd;

    this->_http_status = OK;

    this->_request = RequestInfo();
    this->_request_read_buffer = "";
    this->_request_read_bytes = 0;

    this->_file = NULL;
    this->_file_path = "";
    this->_file_read_bytes = 0;
    this->_file_read_left = 0;
    this->_file_chunk_read_bytes = 0;
    this->_file_size = 0;
    this->_file_read_chunk_buffer[0] = '\0';

    this->_header_sent = false;
    this->_file_opened = false;
    this->_error_response = false;

    this->_event_status = Reading;
    this->_event_sub_status = ReadingRequest;

}

Event::~Event() {
    if (_file != NULL) {
        fclose(_file);
    }
    close(this->getClientFd());
}


void Event::clear_file_info() {
    this->setFileOpened(false);
    this->setFile(NULL);
    this->setFilePath("");
    this->setFileReadBytes(0);
    this->setFileChunkReadBytes(0);
    this->setFileSize(0);
}



int Event::getServerFd() const {
    return _server_fd;
}

void Event::setServerFd(int serverFd) {
    _server_fd = serverFd;
}

int Event::getClientFd() const {
    return _client_fd;
}

void Event::setClientFd(int clientFd) {
    _client_fd = clientFd;
}

const RequestInfo &Event::getRequest() const {
    return _request;
}

void Event::setRequest(const RequestInfo &request) {
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

Server Event::getServer() const {
    return _server;
}

void Event::setServer(Server server) {
    _server = server;
}

event_http_status_enum_t Event::getHttpStatus() const {
    return _http_status;
}

void Event::setHttpStatus(event_http_status_enum_t httpStatus) {
    _http_status = httpStatus;
}

Location Event::getLocation() const {
    return _location;
}

void Event::setLocation(Location location) {
    _location = location;
}

bool Event::isHeaderSent() const {
    return _header_sent;
}

void Event::setHeaderSent(bool headerSent) {
    _header_sent = headerSent;
}

bool Event::isFileOpened() const {
    return _file_opened;
}

void Event::setFileOpened(bool fileOpened) {
    _file_opened = fileOpened;
}

bool Event::isErrorResponse() const {
    return _error_response;
}

void Event::setErrorResponse(bool errorResponse) {
    _error_response = errorResponse;
}


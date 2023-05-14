//
// Created by rmore on 09/04/2023.
//

#include "../../includes/domain/Event.h"

Event::Event(int server_fd, int client_fd) {
    this->_client_fd = client_fd;
    this->_server_fd = server_fd;

    this->_http_status = OK;

    this->_request = RequestData();
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
    this->_forced_redirect = false;


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

int Event::getClientFd() const {
    return _client_fd;
}

void Event::setClientFd(int clientFd) {
    _client_fd = clientFd;
}

int Event::getServerFd() const {
    return _server_fd;
}

void Event::setServerFd(int serverFd) {
    _server_fd = serverFd;
}

Event::event_http_status Event::getHttpStatus() const {
    return _http_status;
}

void Event::setHttpStatus(Event::event_http_status httpStatus) {
    _http_status = httpStatus;
}

const std::string &Event::getRequestReadBuffer() const {
    return _request_read_buffer;
}

void Event::setRequestReadBuffer(const std::string &requestReadBuffer) {
    _request_read_buffer = requestReadBuffer;
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

const std::string &Event::getFilePath() const {
    return _file_path;
}

void Event::setFilePath(const std::string &filePath) {
    _file_path = filePath;
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

Event::event_status Event::getEventStatus() const {
    return _event_status;
}

void Event::setEventStatus(Event::event_status eventStatus) {
    _event_status = eventStatus;
}

Event::event_sub_status Event::getEventSubStatus() const {
    return _event_sub_status;
}

void Event::setEventSubStatus(Event::event_sub_status eventSubStatus) {
    _event_sub_status = eventSubStatus;
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

bool Event::isForcedRedirect() const {
    return _forced_redirect;
}

void Event::setForcedRedirect(bool forcedRedirect) {
    _forced_redirect = forcedRedirect;
}

const std::string &Event::getForcedRedirectLocation() const {
    return _forced_redirect_location;
}

void Event::setForcedRedirectLocation(const std::string &forcedRedirectLocation) {
    _forced_redirect_location = forcedRedirectLocation;
}

const Server &Event::getServer() const {
    return _server;
}

void Event::setServer(const Server &server) {
    _server = server;
}

const Location &Event::getLocation() const {
    return _location;
}

void Event::setLocation(const Location &location) {
    _location = location;
}

const RequestData &Event::getRequest() const {
    return _request;
}

void Event::setRequest(const RequestData &request) {
    _request = request;
}

const std::string &Event::getRemainingReadBuffer() const {
    return _remaining_read_buffer;
}

void Event::setRemainingReadBuffer(const std::string &remainingReadBuffer) {
    _remaining_read_buffer = remainingReadBuffer;
}





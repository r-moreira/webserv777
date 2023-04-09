//
// Created by rmore on 09/04/2023.
//

#include "../../includes/domain/Event.h"

Event::Event(int client_fd) {
    this->client_fd = client_fd;
    this->event_status = Reading;
    this->request = Request();
    this->read_bytes = 0;
    this->read_left = 0;
    this->read_buffer = "";
    this->file_path = "";
    this->write_iteration = 0;
    this->file = NULL;
}

int Event::getWriteIteration() const {
    return write_iteration;
}

void Event::setWriteIteration(int writeIteration) {
    write_iteration = writeIteration;
}

int Event::getClientFd() const {
    return client_fd;
}

void Event::setClientFd(int clientFd) {
    client_fd = clientFd;
}

const std::string &Event::getFilePath() const {
    return file_path;
}

void Event::setFilePath(const std::string &filePath) {
    file_path = filePath;
}

size_t Event::getReadBytes() const {
    return read_bytes;
}

void Event::setReadBytes(size_t readBytes) {
    read_bytes = readBytes;
}

size_t Event::getReadLeft() const {
    return read_left;
}

void Event::setReadLeft(size_t readLeft) {
    read_left = readLeft;
}

const std::string &Event::getReadBuffer() const {
    return read_buffer;
}

void Event::setReadBuffer(const std::string &readBuffer) {
    read_buffer = readBuffer;
}

const Request &Event::getRequest() const {
    return request;
}

void Event::setRequest(const Request &request) {
    Event::request = request;
}

event_status_t Event::getEventStatus() const {
    return event_status;
}

void Event::setEventStatus(event_status_t eventStatus) {
    event_status = eventStatus;
}

FILE *Event::getFile() const {
    return file;
}

void Event::setFile(FILE *file) {
    Event::file = file;
}

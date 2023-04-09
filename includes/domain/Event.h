//
// Created by rmore on 09/04/2023.
//

#ifndef WEBSERV_EVENT_H
#define WEBSERV_EVENT_H

#include "../webserv.h"

typedef enum EVENT_STATUS {
    Reading,
    Writing,
    Ended
} event_status_t;

class Event {

private:
    int write_iteration;
    int client_fd;
    std::string file_path;
    size_t read_bytes;
    size_t read_left;
    std::string read_buffer;
    Request request;
    event_status_t event_status;
    FILE *file;

public:
    Event(int client_fd);

    int getWriteIteration() const;

    void setWriteIteration(int writeIteration);

    int getClientFd() const;

    void setClientFd(int clientFd);

    const std::string &getFilePath() const;

    void setFilePath(const std::string &filePath);

    size_t getReadBytes() const;

    void setReadBytes(size_t readBytes);

    size_t getReadLeft() const;

    void setReadLeft(size_t readLeft);

    const std::string &getReadBuffer() const;

    void setReadBuffer(const std::string &readBuffer);

    const Request &getRequest() const;

    void setRequest(const Request &request);

    event_status_t getEventStatus() const;

    void setEventStatus(event_status_t eventStatus);

    FILE *getFile() const;

    void setFile(FILE *file);
};


#endif //WEBSERV_EVENT_H

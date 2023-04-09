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
    int _write_iteration;
    int _client_fd;
    std::string _file_path;
    size_t _read_bytes;
    size_t _read_left;
    std::string _read_buffer;
    Request _request;
    event_status_t _event_status;
    FILE *_file;

public:
    Event(int client_fd);

    ~Event();

    void process_event();

    void read_request();

    Request parse_request();

    int open_file();

    void write_response();

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

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

typedef enum EVENT_SUB_STATUS {
    ReadingRequest,
    ParsingRequest,
    OpeningFile,
    WritingResponseHeaders,
    UploadingFile
} event_sub_status_t;


class Event {

private:
    int _client_fd;
    std::string _file_path;
    FILE *_file;
    size_t _file_size;
    size_t _read_bytes;
    size_t _read_left;
    std::string _read_buffer;
    Request _request;
    event_status_t _event_status;
    event_sub_status_t _event_sub_status;
public:
    Event(int client_fd);

    ~Event();

    void process_event();

    void read_request();

    void write_response_headers();

    void parse_request();

    void open_file();

    void upload_file();

    size_t getFileSize() const;

    void setFileSize(size_t fileSize);

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

    void setEventSubStatus(event_sub_status_t eventSubStatus);

    event_sub_status_t getEventSubStatus() const;
};


#endif //WEBSERV_EVENT_H

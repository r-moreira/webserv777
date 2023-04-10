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

#define FILE_READ_CHUNK_SIZE 30720

class Event {

private:
    int _client_fd;

    std::string _request_read_buffer;
    size_t _request_read_bytes;

    FILE *_file;
    std::string _file_path;
    size_t _file_read_bytes;
    size_t _file_read_left;
    size_t _file_chunk_read_bytes;
    size_t _file_size;
    char _file_read_chunk_buffer[FILE_READ_CHUNK_SIZE];

    event_status_t _event_status;
    event_sub_status_t _event_sub_status;

public:
    Request _request;

    Event(int client_fd);

    ~Event();

    int getClientFd() const;

    void setClientFd(int clientFd);

    const Request &getRequest() const;

    void setRequest(const Request &request);

    const std::string &getRequestReadBuffer() const;

    void setRequestReadBuffer(const std::string &requestReadBuffer);

    const std::string &getFilePath() const;

    void setFilePath(const std::string &filePath);

    size_t getRequestReadBytes() const;

    void setRequestReadBytes(size_t requestReadBytes);

    FILE *getFile() const;

    void setFile(FILE *file);

    size_t getFileReadBytes() const;

    void setFileReadBytes(size_t fileReadBytes);

    size_t getFileReadLeft() const;

    void setFileReadLeft(size_t fileReadLeft);

    size_t getFileChunkReadBytes() const;

    void setFileChunkReadBytes(size_t fileChunkReadBytes);

    size_t getFileSize() const;

    void setFileSize(size_t fileSize);

    const char *getFileReadChunkBuffer() const;

    event_status_t getEventStatus() const;

    void setEventStatus(event_status_t eventStatus);

    event_sub_status_t getEventSubStatus() const;

    void setEventSubStatus(event_sub_status_t eventSubStatus);
};


#endif //WEBSERV_EVENT_H

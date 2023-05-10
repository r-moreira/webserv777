//
// Created by rmore on 09/04/2023.
//

#ifndef WEBSERV_EVENT_H
#define WEBSERV_EVENT_H

#include "../webserv.h"
#include "../parser/RequestInfo.h"
#include "../network/Server.h"


class Event {

private:
    int _client_fd;
    int _server_fd;

    event_http_status_enum_t _http_status;

    std::string _request_read_buffer;
    size_t _request_read_bytes;

    FILE *_file;
    file_type_t _file_type;
    std::string _file_path;
    size_t _file_read_bytes;
    size_t _file_read_left;
    size_t _file_chunk_read_bytes;
    size_t _file_size;
    char _file_read_chunk_buffer[FILE_READ_CHUNK_SIZE];

    event_status_t _event_status;
    event_sub_status_t _event_sub_status;

    Location _location;
    Server _server;
public:
    RequestInfo _request;

    Event(int server_fd, int client_fd);

    ~Event();

    int getServerFd() const;

    void setServerFd(int serverFd);

    int getClientFd() const;

    void setClientFd(int clientFd);

    Server getServer() const;

    void setServer(Server server);

    Location getLocation() const;

    void setLocation(Location location);

    event_http_status_enum_t getHttpStatus() const;

    void setHttpStatus(event_http_status_enum_t httpStatus);

    const RequestInfo &getRequest() const;

    void setRequest(const RequestInfo &request);

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

    file_type_t getFileType() const;

    void setFileType(file_type_t fileType);
};


#endif //WEBSERV_EVENT_H

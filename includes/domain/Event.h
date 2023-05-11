//
// Created by rmore on 09/04/2023.
//

#ifndef WEBSERV_EVENT_H
#define WEBSERV_EVENT_H

#include "../webserv.h"
#include "../parser/RequestInfo.h"
#include "../config/Server.h"


class Event {

private:
    int _client_fd;
    int _server_fd;

    event_http_status_enum_t _http_status;

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

    bool _header_sent;
    bool _file_opened;
    bool _error_response;
    bool _forced_redirect;

    std::string _forced_redirect_location;

    Server _server;
    Location _location;
public:
    RequestInfo _request;

    Event(int server_fd, int client_fd);

    ~Event();

    void clear_file_info();

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

    bool isHeaderSent() const;

    void setHeaderSent(bool headerSent);

    bool isFileOpened() const;

    void setFileOpened(bool fileOpened);

    bool isErrorResponse() const;

    void setErrorResponse(bool errorResponse);

    bool isForcedRedirect() const;

    void setForcedRedirect(bool forcedRedirect);

    const std::string &getForcedRedirectLocation() const;

    void setForcedRedirectLocation(const std::string &forcedRedirectLocation);
};


#endif //WEBSERV_EVENT_H

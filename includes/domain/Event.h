//
// Created by rmore on 09/04/2023.
//

#ifndef WEBSERV_EVENT_H
#define WEBSERV_EVENT_H

#include "../webserv.h"
#include "../config/Server.h"
#include "../model/RequestData.h"


class Event {
public:
    enum event_status {
        Reading,
        Writing,
        Ended
    };

    enum event_sub_status {
        ReadingRequest,
        ParsingRequest,
        ChoosingServer,
        ChoosingLocation,
        ValidatingConstraints,
        DefiningResponseState,
        SendingResponseFile,
        SendingUploadResponse,
        SendingRedirectionResponse,
        SendingDirectoryResponse,
        SendingErrorResponse
    };

    enum event_http_status {
        OK = 200,
        CREATED = 201, //Upload
        NO_CONTENT = 204, //Delete
        BAD_REQUEST = 400,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        PAYLOAD_TOO_LARGE = 413,
        CLIENT_CLOSED_REQUEST = 499,
        INTERNAL_SERVER_ERROR = 500,
    };

private:
    int _client_fd;
    int _server_fd;

    event_http_status _http_status;

    std::string _request_read_buffer;
    std::string _remaining_read_buffer;
public:
    const std::string &getRemainingReadBuffer() const;

    void setRemainingReadBuffer(const std::string &remainingReadBuffer);

private:
    size_t _request_read_bytes;

    FILE *_file;
    std::string _file_path;
    size_t _file_read_bytes;
    size_t _file_read_left;
    size_t _file_chunk_read_bytes;
    size_t _file_size;
    char _file_read_chunk_buffer[FILE_READ_CHUNK_SIZE];

    Event::event_status _event_status;
    Event::event_sub_status _event_sub_status;

    bool _header_sent;
    bool _file_opened;
    bool _error_response;
    bool _forced_redirect;

    std::string _forced_redirect_location;

    Server _server;
    Location _location;

public:
    RequestData _request;

    Event(int server_fd, int client_fd);

    ~Event();

    void clear_file_info();

    int getClientFd() const;

    void setClientFd(int clientFd);

    int getServerFd() const;

    void setServerFd(int serverFd);

    event_http_status getHttpStatus() const;

    void setHttpStatus(event_http_status httpStatus);

    const std::string &getRequestReadBuffer() const;

    void setRequestReadBuffer(const std::string &requestReadBuffer);

    size_t getRequestReadBytes() const;

    void setRequestReadBytes(size_t requestReadBytes);

    FILE *getFile() const;

    void setFile(FILE *file);

    const std::string &getFilePath() const;

    void setFilePath(const std::string &filePath);

    size_t getFileReadBytes() const;

    void setFileReadBytes(size_t fileReadBytes);

    size_t getFileReadLeft() const;

    void setFileReadLeft(size_t fileReadLeft);

    size_t getFileChunkReadBytes() const;

    void setFileChunkReadBytes(size_t fileChunkReadBytes);

    size_t getFileSize() const;

    void setFileSize(size_t fileSize);

    const char *getFileReadChunkBuffer() const;

    event_status getEventStatus() const;

    void setEventStatus(event_status eventStatus);

    event_sub_status getEventSubStatus() const;

    void setEventSubStatus(event_sub_status eventSubStatus);

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

    const Server &getServer() const;

    void setServer(const Server &server);

    const Location &getLocation() const;

    void setLocation(const Location &location);

    const RequestData &getRequest() const;

    void setRequest(const RequestData &request);

};
#endif //WEBSERV_EVENT_H

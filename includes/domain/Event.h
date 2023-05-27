//
// Created by rmore on 09/04/2023.
//

#ifndef WEBSERV_EVENT_H
#define WEBSERV_EVENT_H

#include "../webserv.h"
#include "../config/Server.h"
#include "../model/RequestData.h"
#include "../cgi/Exec.h"

class Event {

public:
    struct Status {
        enum event_status {
            Reading,
            Writing,
            Ended
        };
    };

    struct SubStatus {
        enum event_sub_status {
            ReadingRequest,
            ParsingRequest,
            ChoosingServer,
            ChoosingLocation,
            ValidatingConstraints,
            DefiningResponseState,
            SendingResponseFile,
            SendingUploadResponse,
            SendingDeleteResponse,
            SendingRedirectionResponse,
            SendingAutoIndexResponse,
            SendingCGIResponse,
            SendingDirectoryResponse,
            SendingErrorResponse
        };
    };

    struct HttpStatus {
        enum event_http_status {
            OK = 200,
            CREATED = 201,
            NO_CONTENT = 204,
            BAD_REQUEST = 400,
            FORBIDDEN = 403,
            NOT_FOUND = 404,
            METHOD_NOT_ALLOWED = 405,
            PAYLOAD_TOO_LARGE = 413,
            CLIENT_CLOSED_REQUEST = 499,
            INTERNAL_SERVER_ERROR = 500,
            NOT_IMPLEMENTED = 501,
            LOOP_DETECTED = 508
        };
    };

private:
    int _client_fd;
    int _server_fd;
    int _cgi_fd_out;
    int _cgi_fd_in;
    int _server_cgi_fd_out;
    int _server_cgi_fd_in;

    HttpStatus::event_http_status _http_status;

    std::string _request_read_buffer;
    std::string _remaining_read_buffer;
    size_t _remaining_read_buffer_size;
    size_t _request_read_bytes;

    size_t _remaining_file_bytes;
    char _content_chunk_buffer[BODY_READ_BUFFER_SIZE];

    FILE *_file;
    std::string _file_path;
    size_t _file_read_bytes;
    size_t _file_read_left;
    size_t _file_chunk_read_bytes;
    size_t _file_size;
    char _file_read_chunk_buffer[FILE_READ_CHUNK_SIZE];

    Event::Status::event_status _event_status;
    Event::SubStatus::event_sub_status _event_sub_status;

    bool _header_sent;
    bool _file_opened;
    bool _error_response;
    bool _forced_redirect;
    bool _remaining_read_bytes_writed;
    bool _is_cgi_set;

    std::string _forced_redirect_location;

    Exec *_cgi;
    char *_cgi_path;
    char * const* _cgi_envp;

    Server _server;
    Location _location;

public:
    RequestData _request;

    Event(int server_fd, int client_fd);

    ~Event();

    void clear_file_info();

    Event::HttpStatus::event_http_status convert_int_to_http_status(int status);

    int getClientFd() const;

    void setClientFd(int clientFd);

    int getServerFd() const;

    void setServerFd(int serverFd);

    HttpStatus::event_http_status getHttpStatus() const;

    void setHttpStatus(HttpStatus::event_http_status httpStatus);

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

    Status::event_status getEventStatus() const;

    void setEventStatus(Status::event_status eventStatus);

    SubStatus::event_sub_status getEventSubStatus() const;

    void setEventSubStatus(SubStatus::event_sub_status eventSubStatus);

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

    const std::string &getRemainingReadBuffer() const;

    void setRemainingReadBuffer(const std::string &remainingReadBuffer);

    size_t getRemainingFileBytes() const;

    void setRemainingFileBytes(size_t remainingFileBytes);

    bool isRemainingReadBytesWrited() const;

    void setRemainingReadBytesWrited(bool remainingReadBytesWrited);

    const char *getContentChunkBuffer() const;

    size_t getRemainingReadBufferSize() const;

    void setRemainingReadBufferSize(size_t remainingReadBufferSize);

    int getCgiFdOut() const;

    void setCgiFdOut(int cgiFdOut);

    int getCgiFdIn() const;

    void setCgiFdIn(int cgiFdIn);

    bool isCgiSet() const;

    void setIsCgiSet(bool isCgiSet);

    Exec *getCgi() const;

    void setCgi(Exec *cgi);

    char *getCgiPath() const;

    void setCgiPath(char *cgiPath);

    char* const* getEnvp() const;

    void setEnvp(char* const* cgi_envp);

    int getServerCgiFdOut() const;

    void setServerCgiFdOut(int serverCgiFdOut);

    int getServerCgiFdIn() const;

    void setServerCgiFdIn(int serverCgiFdIn);
};
#endif //WEBSERV_EVENT_H

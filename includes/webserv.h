//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_WEBSERV_H
#define WEBSERV_WEBSERV_H

#include <iostream>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include <sys/sendfile.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fstream>
#include <fcntl.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */

#define MAX_EPOLL_EVENTS 64
#define READ_BUFFER_SIZE 8192
#define FILE_READ_CHUNK_SIZE 30720

typedef enum EVENT_STATUS {
    Reading,
    Writing,
    Ended
} event_status_t;

typedef enum EVENT_SUB_STATUS {
    ReadingRequest,
    ParsingRequest,
    ChoosingServer,
    ChoosingLocation,
    ValidatingConstraints,
    OpeningFile,
    WritingFileResponseHeaders,
    WritingResponseFile,
    WritingErrorHeaders,
    WritingErrorPage
} event_sub_status_t;

typedef enum FILE_TYPE {
    Regular,
    CGI,
    AutoIndexPage,
    DirectoryPage,
    DefaultPage,
    ErrorPage
} file_type_t;


typedef enum EVENT_HTTP_STATUS_ENUM {
    OK = 200,
    CREATED = 201,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    FORBIDDEN = 403,
    METHOD_NOT_ALLOWED = 405,
    PAYLOAD_TOO_LARGE = 413,
    CLIENT_CLOSED_REQUEST = 499,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
} event_http_status_enum_t;



#endif //WEBSERV_WEBSERV_H

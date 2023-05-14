//
// Created by rmore on 13/05/2023.
//

#ifndef WEBSERV_REQUESTPARSER_H
#define WEBSERV_REQUESTPARSER_H

#include "../webserv.h"
#include "../model/RequestData.h"

class RequestParser {

private:
    size_t contentSize;

public:
    enum ParseResult {
        ParsingCompleted,
        ParsingIncompleted,
        ParsingError
    };

    enum State {
        RequestMethodStart,
        RequestMethod,
        RequestUriStart,
        RequestUri,
        RequestHttpVersion_h,
        RequestHttpVersion_ht,
        RequestHttpVersion_htt,
        RequestHttpVersion_http,
        RequestHttpVersion_slash,
        RequestHttpVersion_majorStart,
        RequestHttpVersion_major,
        RequestHttpVersion_minorStart,
        RequestHttpVersion_minor,
        ResponseHttpVersion_newLine,

        HeaderLineStart,
        HeaderLws,
        HeaderName,
        SpaceBeforeHeaderValue,
        HeaderValue,

        ExpectingNewline_2,
        ExpectingNewline_3,

        Post
    } state;

    RequestParser();
    ~RequestParser();

    ParseResult parse(RequestData &req, const char *begin, const char *end);

private:
    static bool checkIfConnection(const RequestData::HeaderItem &item);

    ParseResult consume(RequestData &req, const char *begin, const char *end);

    inline bool isChar(int c);

    inline bool isControl(int c);

    inline bool isSpecial(int c);

    inline bool isDigit(int c);


};


#endif //WEBSERV_REQUESTPARSER_H

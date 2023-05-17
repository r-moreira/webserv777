//
// Created by rmore on 13/05/2023.
//

#ifndef WEBSERV_REQUESTPARSER_H
#define WEBSERV_REQUESTPARSER_H

#include "../webserv.h"
#include "../model/RequestData.h"

class RequestParser {

private:
    size_t _content_size;

public:
    enum ParseState {
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

        ExpectingNewline,
        ExpectingNewline_2,

        Post,
        ExpectingNewline_3,
        Boundary,
        ContentDisposition,
        ExpectingNewline_4,
        FileContentType,
        ExpectingNewline_5,

        ParsingCompleted,
        ParsingError,
        ParsingIncompleted,
    } state;

    RequestParser();
    ~RequestParser();

    ParseState parse(RequestData &req, char c);

private:
    static bool checkIfConnection(const RequestData::HeaderItem &item);;

    static bool checkIfContentType(const RequestData::HeaderItem &item);

    ParseState consume(RequestData &req, char c);

    static inline bool isChar(int c);

    static inline bool isControl(int c);

    static inline bool isSpecial(int c);

    static inline bool isDigit(int c);
};


#endif //WEBSERV_REQUESTPARSER_H

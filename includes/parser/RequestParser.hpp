//
// Created by rmore on 13/05/2023.
//

#ifndef WEBSERV_REQUESTPARSER_HPP
#define WEBSERV_REQUESTPARSER_HPP

#include "../webserv.hpp"
#include "../model/RequestData.hpp"

class RequestParser {

public:
    struct ParseState {
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

            ExpectingNewline,
            ExpectingNewline_2,

            PostContent,

            PostFileUploadBoundary,
            ExpectingNewline_3,
            ContentDisposition,
            ExpectingNewline_4,
            FileContentType,
            SpaceBeforeFileContentTypeValue,
            FileContentTypeValue,
            ExpectingNewline_5,

            ExpectingLineEnd,
            ExpectingNewline_6,

            ParsingCompleted,
            ParsingError,
            ParsingIncompleted,
        };
    };

    RequestParser();
    ~RequestParser();

    ParseState::State parse(RequestData &req, char c);

private:
    size_t _content_size;
    ParseState::State _state;

    static bool checkIfConnection(const RequestData::HeaderItem &item);;

    static bool checkIfContentType(const RequestData::HeaderItem &item);

    ParseState::State consume(RequestData &req, char c);

    static inline bool isChar(int c);

    static inline bool isControl(int c);

    static inline bool isSpecial(int c);

    static inline bool isDigit(int c);
};


#endif //WEBSERV_REQUESTPARSER_HPP

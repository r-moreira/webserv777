//
// Created by rmore on 13/05/2023.
//

#include "../../includes/parser/RequestParser.h"

RequestParser::RequestParser() : state(RequestMethodStart), _content_size(0){}

RequestParser::~RequestParser() {}

RequestParser::ParseState RequestParser::parse(RequestData &req, char c) {
    return consume(req, c);
}

RequestParser::ParseState RequestParser::consume(RequestData &req, char input) {

    switch (state) {
        case RequestMethodStart:
            if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParsingError;
            } else {
                state = RequestMethod;
                req.methodPushBack(input);
            }
            break;
        case RequestMethod:
            if (input == ' ') {
                state = RequestUriStart;
            } else if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParsingError;
            } else {
                req.methodPushBack(input);
            }
            break;
        case RequestUriStart:
            if (isControl(input)) {
                return ParsingError;
            } else {
                state = RequestUri;
                req.uriPushBack(input);
            }
            break;
        case RequestUri:
            if (input == ' ') {
                state = RequestHttpVersion_h;
            } else if (input == '\r') {
                req.setVersionMajor(0);
                req.setVersionMinor(9);

                return ParsingCompleted;
            } else if (isControl(input)) {
                return ParsingError;
            } else {
                req.uriPushBack(input);
            }
            break;
        case RequestHttpVersion_h:
            if (input == 'H') {
                state = RequestHttpVersion_ht;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_ht:
            if (input == 'T') {
                state = RequestHttpVersion_htt;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_htt:
            if (input == 'T') {
                state = RequestHttpVersion_http;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_http:
            if (input == 'P') {
                state = RequestHttpVersion_slash;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_slash:
            if (input == '/') {
                req.setVersionMajor(0);
                req.setVersionMinor(0);
                state = RequestHttpVersion_majorStart;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_majorStart:
            if (isDigit(input)) {
                req.setVersionMajor(input - '0');
                state = RequestHttpVersion_major;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_major:
            if (input == '.') {
                state = RequestHttpVersion_minorStart;
            } else if (isDigit(input)) {
                req.setVersionMajor(req.getVersionMajor() * 10 + input - '0');
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_minorStart:
            if (isDigit(input)) {
                req.setVersionMinor(input - '0');
                state = RequestHttpVersion_minor;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_minor:
            if (input == '\r') {
                state = ResponseHttpVersion_newLine;
            } else if (isDigit(input)) {
                req.setVersionMinor(req.getVersionMinor() * 10 + input - '0');
            } else {
                return ParsingError;
            }
            break;
        case ResponseHttpVersion_newLine:
            if (input == '\n') {
                state = HeaderLineStart;
            } else {
                return ParsingError;
            }
            break;
        case HeaderLineStart:
            if (input == '\r') {
                state = ExpectingNewline_2;
            } else if (!req.getHeaders().empty() && (input == ' ' || input == '\t')) {
                state = HeaderLws;
            } else if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParsingError;
            } else {
               req.headerItemPushBackNameReserve(input);
               state = HeaderName;
            }
            break;
        case HeaderLws:
            if (input == '\r') {
                state = ExpectingNewline;
            } else if (input == ' ' || input == '\t') {
            } else if (isControl(input)) {
                return ParsingError;
            } else {
                state = HeaderValue;
                req.headerItemPushBackValue(input);
            }
            break;
        case HeaderName:
            if (input == ':') {
                state = SpaceBeforeHeaderValue;
            } else if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParsingError;
            } else {
                req.headerItemPushBackName(input);
            }
            break;
        case SpaceBeforeHeaderValue:
            if (input == ' ') {
                state = HeaderValue;
            } else {
                return ParsingError;
            }
            break;
        case HeaderValue:
            if (input == '\r') {
                if (req.getMethod() == "POST" || req.getMethod() == "PUT") {
                    RequestData::HeaderItem h = req.getHeaders().back();

                    if (strcasecmp(h.name.c_str(), "Content-Length") == 0) {
                        _content_size = atoi(h.value.c_str());
                        req.reserveContent(_content_size);
                    } else if (strcasecmp(h.name.c_str(), "Transfer-Encoding") == 0) {
                        if (strcasecmp(h.value.c_str(), "chunked") == 0)
                            return ParsingError;
                    }
                }
                state = ExpectingNewline;
            } else if (isControl(input)) {
                return ParsingError;
            } else {
                req.headerItemPushBackValue(input);
            }
            break;
        case ExpectingNewline:
            if (input == '\n') {
                state = HeaderLineStart;
            } else {
                return ParsingError;
            }
            break;
        case ExpectingNewline_2: {
            std::vector<RequestData::HeaderItem> headers = req.getHeaders();
            std::vector<RequestData::HeaderItem>::iterator it = std::find_if(headers.begin(),
                                                                             headers.end(),
                                                                             checkIfConnection);
            if (it != headers.end()) {
                if (strcasecmp(it->value.c_str(), "Keep-Alive") == 0) {
                    req.setKeepAlive(true);
                } else {
                    req.setKeepAlive(false);
                }
            } else {
                if (req.getVersionMajor() > 1 || (req.getVersionMajor() == 1 && req.getVersionMinor() == 1))
                    req.setKeepAlive(true);
            }
            if (_content_size == 0) {
                if (input == '\n')
                    return ParsingCompleted;
                else
                    return ParsingError;
            } else {
                //TODO: Checar se é um request POST CGI, caso for, fazer o parse do content, caso contrário, prosseguir com o código abaixo

                std::vector<RequestData::HeaderItem> headers_ = req.getHeaders();
                std::vector<RequestData::HeaderItem>::iterator it_ = std::find_if(headers.begin(),
                                                                                 headers.end(),
                                                                                 checkIfContentType);
                if (it_ != headers_.end()) {
                    if (it_->value.rfind("multipart/form-data;", 0) == 0) {
                        req.setIsFileUpload(true);
                        req.setBoundary(it_->value.substr(it_->value.find("boundary=") + 9));

                    } else {
                        return ParsingError;
                    }

                } else {
                    return ParsingError;
                }

                state = PostFileUploadBoundary;
            }
            break;
        }
        case PostFileUploadBoundary:
            if (input == '\r') {
                state = ExpectingNewline_3;
            }
            _content_size--;
            break;
        case ExpectingNewline_3:
            if (input == '\n') {
                state = ContentDisposition;
                _content_size--;
            } else {
                return ParsingError;
            }
            break;
        case ContentDisposition:
            if (input == '\r') {
                state = ExpectingNewline_4;
            } else {
                req.contentDispositionPushBack(input);
            }
            _content_size--;
            break;
        case ExpectingNewline_4:
            if (input == '\n') {
                state = FileContentType;
                _content_size--;
            } else {
                return ParsingError;
            }
            break;
        case FileContentType:
            if (input == '\r') {
                return ParsingError;
            }
            if (input == ':') {
                state = SpaceBeforeFileContentTypeValue;
            }
            _content_size--;
            break;
        case SpaceBeforeFileContentTypeValue:
            if (input == ' ') {
                state = FileContentTypeValue;
                _content_size--;
            } else {
                return ParsingError;
            }
            break;
        case FileContentTypeValue:
            if (input == '\r') {
                state = ExpectingNewline_5;
            } else {
                req.uploadFileTypePushBack(input);
            }
            _content_size--;
            break;
        case ExpectingNewline_5:
            if (input == '\n') {
                state = ExpectingLineEnd;
                _content_size--;
             } else {
                return ParsingError;
            }
            break;
        case ExpectingLineEnd:
            if (input == '\r') {
                state = ExpectingNewline_6;
                _content_size--;
            } else {
                return ParsingError;
            }
            break;
        case ExpectingNewline_6:
            if (input == '\n') {
                state = ParsingCompleted;
                _content_size--;
                //Calculating file size without extra body info
                size_t endBoundary =  req.getBoundary().size() + 2; //+2 = "--" at end of boundary
                req.setFileUploadRemainingBytes(_content_size - endBoundary - 6); //-6 = after headers "\r\n\r\n" and after body "\r\n", before end boundary
           } else {
                return ParsingError;
            }
            break;
        default:
            return ParsingIncompleted;
    }

    return state;
}

bool RequestParser::checkIfConnection(const RequestData::HeaderItem &item) {
    return strcasecmp(item.name.c_str(), "Connection") == 0;
}

bool RequestParser::checkIfContentType(const RequestData::HeaderItem &item) {
    return strcasecmp(item.name.c_str(), "Content-Type") == 0;
}

bool RequestParser::isChar(int c) {
    return c >= 0 && c <= 127;
}

bool RequestParser::isControl(int c) {
    return (c >= 0 && c <= 31) || (c == 127);
}

bool RequestParser::isDigit(int c) {
    return c >= '0' && c <= '9';
}

bool RequestParser::isSpecial(int c) {
    switch (c) {
        case '(':
        case ')':
        case '<':
        case '>':
        case '@':
        case ',':
        case ';':
        case ':':
        case '\\':
        case '"':
        case '/':
        case '[':
        case ']':
        case '?':
        case '=':
        case '{':
        case '}':
        case ' ':
        case '\t':
            return true;
        default:
            return false;
    }
}

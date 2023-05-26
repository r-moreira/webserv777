//
// Created by rmore on 13/05/2023.
//

#include "../../includes/parser/RequestParser.h"

RequestParser::RequestParser() : _content_size(0), _state(ParseState::RequestMethodStart) {}

RequestParser::~RequestParser() {}

RequestParser::ParseState::State RequestParser::parse(RequestData &req, char c) {
    return consume(req, c);
}

RequestParser::ParseState::State RequestParser::consume(RequestData &req, char input) {

    switch (_state) {
        case ParseState::RequestMethodStart:
            if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParseState::ParsingError;
            } else {
                _state = ParseState::RequestMethod;
                req.methodPushBack(input);
            }
            break;
        case ParseState::RequestMethod:
            if (input == ' ') {
                _state = ParseState::RequestUriStart;
            } else if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParseState::ParsingError;
            } else {
                req.methodPushBack(input);
            }
            break;
        case ParseState::RequestUriStart:
            if (isControl(input)) {
                return ParseState::ParsingError;
            } else {
                _state = ParseState::RequestUri;
                req.uriPushBack(input);
            }
            break;
        case ParseState::RequestUri:
            if (input == ' ') {
                _state = ParseState::RequestHttpVersion_h;
            } else if (input == '\r') {
                req.setVersionMajor(0);
                req.setVersionMinor(9);

                return ParseState::ParsingCompleted;
            } else if (isControl(input)) {
                return ParseState::ParsingError;
            } else {
                req.uriPushBack(input);
            }
            break;
        case ParseState::RequestHttpVersion_h:
            if (input == 'H') {
                _state = ParseState::RequestHttpVersion_ht;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::RequestHttpVersion_ht:
            if (input == 'T') {
                _state = ParseState::RequestHttpVersion_htt;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::RequestHttpVersion_htt:
            if (input == 'T') {
                _state = ParseState::RequestHttpVersion_http;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::RequestHttpVersion_http:
            if (input == 'P') {
                _state = ParseState::RequestHttpVersion_slash;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::RequestHttpVersion_slash:
            if (input == '/') {
                req.setVersionMajor(0);
                req.setVersionMinor(0);
                _state = ParseState::RequestHttpVersion_majorStart;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::RequestHttpVersion_majorStart:
            if (isDigit(input)) {
                req.setVersionMajor(input - '0');
                _state = ParseState::RequestHttpVersion_major;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::RequestHttpVersion_major:
            if (input == '.') {
                _state = ParseState::RequestHttpVersion_minorStart;
            } else if (isDigit(input)) {
                req.setVersionMajor(req.getVersionMajor() * 10 + input - '0');
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::RequestHttpVersion_minorStart:
            if (isDigit(input)) {
                req.setVersionMinor(input - '0');
                _state = ParseState::RequestHttpVersion_minor;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::RequestHttpVersion_minor:
            if (input == '\r') {
                _state = ParseState::ResponseHttpVersion_newLine;
            } else if (isDigit(input)) {
                req.setVersionMinor(req.getVersionMinor() * 10 + input - '0');
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::ResponseHttpVersion_newLine:
            if (input == '\n') {
                _state = ParseState::HeaderLineStart;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::HeaderLineStart:
            if (input == '\r') {
                _state = ParseState::ExpectingNewline_2;
            } else if (!req.getHeaders().empty() && (input == ' ' || input == '\t')) {
                _state = ParseState::HeaderLws;
            } else if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParseState::ParsingError;
            } else {
               req.headerItemPushBackNameReserve(input);
                _state = ParseState::HeaderName;
            }
            break;
        case ParseState::HeaderLws:
            if (input == '\r') {
                _state = ParseState::ExpectingNewline;
            } else if (input == ' ' || input == '\t') {
            } else if (isControl(input)) {
                return ParseState::ParsingError;
            } else {
                _state = ParseState::HeaderValue;
                req.headerItemPushBackValue(input);
            }
            break;
        case ParseState::HeaderName:
            if (input == ':') {
                _state = ParseState::SpaceBeforeHeaderValue;
            } else if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParseState::ParsingError;
            } else {
                req.headerItemPushBackName(input);
            }
            break;
        case ParseState::SpaceBeforeHeaderValue:
            if (input == ' ') {
                _state = ParseState::HeaderValue;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::HeaderValue:
            if (input == '\r') {
                if (req.getMethod() == "POST" || req.getMethod() == "PUT") {
                    RequestData::HeaderItem h = req.getHeaders().back();

                    if (strcasecmp(h.name.c_str(), "Content-Length") == 0) {
                        _content_size = strtol(h.value.c_str(), NULL, 10);
                        req.reserveContent(_content_size);
                    }
                }
                _state = ParseState::ExpectingNewline;
            } else if (isControl(input)) {
                return ParseState::ParsingError;
            } else {
                req.headerItemPushBackValue(input);
            }
            break;
        case ParseState::ExpectingNewline:
            if (input == '\n') {
                _state = ParseState::HeaderLineStart;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::ExpectingNewline_2: {
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
                    return ParseState::ParsingCompleted;
                else
                    return ParseState::ParsingError;
            } else {
                std::vector<RequestData::HeaderItem> headers_ = req.getHeaders();
                std::vector<RequestData::HeaderItem>::iterator it_ = std::find_if(headers.begin(),
                                                                                 headers.end(),
                                                                                 checkIfContentType);
                if (it_ != headers_.end()) {
                    if (it_->value.rfind("multipart/form-data;", 0) == 0) {
                        req.setBoundary(it_->value.substr(it_->value.find("boundary=") + 9));

                        _state = ParseState::PostFileUploadBoundary;
                        break;
                    }
                }

                req.setFileUploadRemainingBytes(_content_size);
                return ParseState::ParsingCompleted;
            }
        }
        case ParseState::PostFileUploadBoundary:
            if (input == '\r') {
                _state = ParseState::ExpectingNewline_3;
            }
            _content_size--;
            break;
        case ParseState::ExpectingNewline_3:
            if (input == '\n') {
                _state = ParseState::ContentDisposition;
                _content_size--;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::ContentDisposition:
            if (input == '\r') {
                _state = ParseState::ExpectingNewline_4;
            } else {
                req.contentDispositionPushBack(input);
            }
            _content_size--;
            break;
        case ParseState::ExpectingNewline_4:
            if (input == '\n') {
                _state = ParseState::FileContentType;
                _content_size--;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::FileContentType:
            if (input == '\r') {
                return ParseState::ParsingError;
            }
            if (input == ':') {
                _state = ParseState::SpaceBeforeFileContentTypeValue;
            }
            _content_size--;
            break;
        case ParseState::SpaceBeforeFileContentTypeValue:
            if (input == ' ') {
                _state = ParseState::FileContentTypeValue;
                _content_size--;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::FileContentTypeValue:
            if (input == '\r') {
                _state = ParseState::ExpectingNewline_5;
            } else {
                req.uploadFileTypePushBack(input);
            }
            _content_size--;
            break;
        case ParseState::ExpectingNewline_5:
            if (input == '\n') {
                _state = ParseState::ExpectingLineEnd;
                _content_size--;
             } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::ExpectingLineEnd:
            if (input == '\r') {
                _state = ParseState::ExpectingNewline_6;
                _content_size--;
            } else {
                return ParseState::ParsingError;
            }
            break;
        case ParseState::ExpectingNewline_6:
            if (input == '\n') {
                _state = ParseState::ParsingCompleted;
                _content_size--;
                //Calculating file size without extra body info
                size_t endBoundary =  req.getBoundary().size() + 2; //+2 = "--" at end of boundary
                req.setFileUploadRemainingBytes(_content_size - endBoundary - 6); //-6 = after headers "\r\n\r\n" and after body "\r\n", before end boundary
           } else {
                return ParseState::ParsingError;
            }
            break;
        default:
            return ParseState::ParsingIncompleted;
    }

    return _state;
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

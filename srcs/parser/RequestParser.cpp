//
// Created by rmore on 13/05/2023.
//

#include "../../includes/parser/RequestParser.h"

RequestParser::RequestParser() : state(RequestMethodStart), contentSize(0){}

RequestParser::~RequestParser() {}

RequestParser::ParseResult RequestParser::parse(RequestData &req, const char *begin, const char *end) {
    return RequestParser::ParsingError;
}

RequestParser::ParseResult RequestParser::consume(RequestData &req, const char *begin, const char *end) {
    return RequestParser::ParsingError;
}

bool RequestParser::checkIfConnection(const RequestData::HeaderItem &item) {
    return strcasecmp(item.name.c_str(), "Connection") == 0;
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

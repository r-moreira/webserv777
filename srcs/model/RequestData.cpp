//
// Created by rmore on 13/05/2023.
//

#include "../../includes/model/RequestData.h"

RequestData::RequestData(): _versionMajor(0), _versionMinor(0), keepAlive(false) {}

RequestData::~RequestData() {}

std::string RequestData::inspect() const {
    std::stringstream stream;
    stream << _method << " " << _uri << " HTTP/"
           << _versionMajor << "." << _versionMinor << "\n";

    for (std::vector<RequestData::HeaderItem>::const_iterator it = _headers.begin();
         it != _headers.end(); ++it) {
        stream << it->name << ": " << it->value << "\n";
    }

    std::string data(_content.begin(), _content.end());
    stream << data << "\n";
    stream << "+ keep-alive: " << keepAlive << "\n";;
    return stream.str();
}


void RequestData::methodPushBack(char c) {
    _method.push_back(c);
}

void RequestData::uriPushBack(char c) {
    _uri.push_back(c);
}

void RequestData::headerItemPushBackNameReserve(char c) {
    _headers.push_back(RequestData::HeaderItem());
    _headers.back().name.reserve(16);
    _headers.back().value.reserve(16);
    _headers.back().name.push_back(c);
}

void RequestData::headerItemPushBackName(char c) {
    _headers.back().name.push_back(c);
}


void RequestData::headerItemPushBackValue(char c) {
    _headers.back().value.push_back(c);
}

void RequestData::reserveContent(size_t size) {
    _content.reserve(size);
}

void RequestData::contentPushBack(char c) {
    _content.push_back(c);
}

const std::string &RequestData::getMethod() const {
    return _method;
}

const std::string &RequestData::getUri() const {
    return _uri;
}

int RequestData::getVersionMajor() const {
    return _versionMajor;
}

int RequestData::getVersionMinor() const {
    return _versionMinor;
}

const std::vector<RequestData::HeaderItem> &RequestData::getHeaders() const {
    return _headers;
}

const std::vector<char> &RequestData::getContent() const {
    return _content;
}

bool RequestData::isKeepAlive() const {
    return keepAlive;
}

void RequestData::setVersionMajor(int versionMajor) {
    _versionMajor = versionMajor;
}

void RequestData::setVersionMinor(int versionMinor) {
    _versionMinor = versionMinor;
}

void RequestData::setKeepAlive(bool keepAlive) {
    RequestData::keepAlive = keepAlive;
}

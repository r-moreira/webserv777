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

const std::string &RequestData::getMethod() const {
    return _method;
}

void RequestData::setMethod(const std::string &method) {
    _method = method;
}

const std::string &RequestData::getUri() const {
    return _uri;
}

void RequestData::setUri(const std::string &uri) {
    _uri = uri;
}

int RequestData::getVersionMajor() const {
    return _versionMajor;
}

void RequestData::setVersionMajor(int versionMajor) {
    _versionMajor = versionMajor;
}

int RequestData::getVersionMinor() const {
    return _versionMinor;
}

void RequestData::setVersionMinor(int versionMinor) {
    _versionMinor = versionMinor;
}

const std::vector<RequestData::HeaderItem> &RequestData::getHeaders() const {
    return _headers;
}

void RequestData::setHeaders(const std::vector<RequestData::HeaderItem> &headers) {
    _headers = headers;
}

const std::vector<char> &RequestData::getContent() const {
    return _content;
}

void RequestData::setContent(const std::vector<char> &content) {
    _content = content;
}

bool RequestData::isKeepAlive() const {
    return keepAlive;
}

void RequestData::setKeepAlive(bool keepAlive) {
    RequestData::keepAlive = keepAlive;
}

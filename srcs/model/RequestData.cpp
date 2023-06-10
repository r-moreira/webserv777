//
// Created by rmore on 13/05/2023.
//

#include "../../includes/model/RequestData.hpp"

RequestData::RequestData() :
        _version_major(0),
        _version_minor(0),
        _keep_alive(false),
        _remaining_body_bytes(0),
        _content_length(-1){}

RequestData::~RequestData() {}

std::string RequestData::inspect() const {
    std::stringstream stream;
    stream << _method << " " << _uri << " HTTP/"
           << _version_major << "." << _version_minor << std::endl;

    for (std::vector<RequestData::HeaderItem>::const_iterator it = _headers.begin();
         it != _headers.end(); ++it) {
        stream << it->name << ": " << it->value << std::endl;;
    }

    std::string data(_content.begin(), _content.end());
    stream << std::endl << data << std::endl << std::endl;
    stream << "+ keep-alive: " << _keep_alive << std::endl;
    stream << "+ remaining_bytes: " << _remaining_body_bytes << std::endl;
    stream << "+ boundary: " << _boundary << std::endl;
    stream << "+ content_disposition: " << _content_disposition << std::endl;
    stream << "+ upload_file_type: " << _upload_file_type << std::endl;
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

void RequestData::contentDispositionPushBack(char c) {
    _content_disposition.push_back(c);
}

void RequestData::uploadFileTypePushBack(char c) {
    _upload_file_type.push_back(c);
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
    return _version_major;
}

void RequestData::setVersionMajor(int versionMajor) {
    _version_major = versionMajor;
}

int RequestData::getVersionMinor() const {
    return _version_minor;
}

void RequestData::setVersionMinor(int versionMinor) {
    _version_minor = versionMinor;
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
    return _keep_alive;
}

void RequestData::setKeepAlive(bool keepAlive) {
    _keep_alive = keepAlive;
}

size_t RequestData::getBodyRemainingBytes() const {
    return _remaining_body_bytes;
}

void RequestData::setBodyRemainingBytes(size_t remaining_body_bytes) {
    _remaining_body_bytes = remaining_body_bytes;
}

const std::string &RequestData::getBoundary() const {
    return _boundary;
}

void RequestData::setBoundary(const std::string &boundary) {
    _boundary = boundary;
}

const std::string &RequestData::getContentDisposition() const {
    return _content_disposition;
}

void RequestData::setContentDisposition(const std::string &contentDisposition) {
    _content_disposition = contentDisposition;
}

const std::string &RequestData::getUploadFileType() const {
    return _upload_file_type;
}

void RequestData::setUploadFileType(const std::string &uploadFileType) {
    _upload_file_type = uploadFileType;
}

void RequestData::setContentLength(size_t content_size) {
    _content_length = content_size;
}

size_t RequestData::getContentLength() const {
    return _content_length;
}
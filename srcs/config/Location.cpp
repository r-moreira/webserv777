//
// Created by rmore on 07/05/2023.
//

#include "../../includes/config/Location.h"

Location::Location() {
    this->_path = "/";
    this->_limit_except = std::vector<std::string>();
    this->_limit_except.push_back("GET");
    this->_limit_except.push_back("POST");
    this->_limit_except.push_back("DELETE");
    this->_auto_index = false;
    this->_cgi_lock = false;
    this->_upload_lock = false;
    this->_redirect_lock = false;
}

const std::string &Location::getPath() const {
    return _path;
}

void Location::setPath(const std::string &path) {
    _path = path;
}

const std::string &Location::getRoot() const {
    return _root;
}

void Location::setRoot(const std::string &root) {
    _root = root;
}

const std::vector<std::string> &Location::getLimitExcept() const {
    return _limit_except;
}

void Location::setLimitExcept(const std::vector<std::string> &limitExcept) {
    _limit_except = limitExcept;
}

const std::string &Location::getRedirectLocation() const {
    return _redirect_location;
}

void Location::getRedirectLocation(const std::string &redirectLocation) {
    _redirect_location = redirectLocation;
}

const std::string &Location::getCgiPath() const {
    return _cgi_path;
}

void Location::setCgiPath(const std::string &cgiPath) {
    _cgi_path = cgiPath;
}

const std::string &Location::getUploadPath() const {
    return _upload_path;
}

void Location::setUploadPath(const std::string &uploadPath) {
    _upload_path = uploadPath;
}

bool Location::isAutoIndex() const {
    return _auto_index;
}

void Location::setAutoIndex(bool autoIndex) {
    _auto_index = autoIndex;
}

bool Location::isCgiLock() const {
    return _cgi_lock;
}

void Location::setCgiLock(bool cgiLock) {
    _cgi_lock = cgiLock;
}

bool Location::isUploadLock() const {
    return _upload_lock;
}

void Location::setUploadLock(bool uploadLock) {
    _upload_lock = uploadLock;
}

bool Location::isRedirectLock() const {
    return _redirect_lock;
}

void Location::setRedirectLock(bool redirectLock) {
    _redirect_lock = redirectLock;
}

const std::string &Location::getIndex() const {
    return _index;
}

void Location::setIndex(const std::string &index) {
    _index = index;
}


std::ostream &operator<<(std::ostream &os, const Location &location) {
    os
            << "\tPath: " << location.getPath() << std::endl
            << "\tRoot: " << location.getRoot() << std::endl
            << "\tIndex: " << location.getIndex() << std::endl
            << "\tRedirect Url: " << location.getRedirectLocation() << std::endl
        << "\tCgi Path: " << location.getCgiPath() << std::endl
        << "\tUpload Path: " << location.getUploadPath() << std::endl
        << "\tAuto Index: " << location.isAutoIndex() << std::endl
        << "\tCgi Lock: " << location.isCgiLock()
        << " | Upload Lock: " << location.isUploadLock()
        << " | Redirect Lock: " << location.isRedirectLock() << std::endl
        << "\tLimit Except: ";

    for (size_t i = 0; i < location.getLimitExcept().size(); i++) {
        os << location.getLimitExcept()[i] << " ";
        os.flush();
    }

    return os;
}
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
    this->_max_body_size = -1;
    this->_auto_index = AutoIndexOption::NONE;
    this->_cgi_lock = false;
    this->_upload_lock = false;
    this->_redirect_lock = false;
}

Location &Location::operator=(const Location &obj) {
    if (this == &obj)
        return *this;
    this->_path = obj._path;
    this->_root = obj._root;
    this->_index = obj._index;
    this->_limit_except = obj._limit_except;
    this->_redirect_location = obj._redirect_location;
    this->_cgi_path = obj._cgi_path;
    this->_cgi_bin = obj._cgi_bin;
    this->_upload_path = obj._upload_path;
    this->_directory_request_page = obj._directory_request_page;
    this->_error_pages = obj._error_pages;
    this->_max_body_size = obj._max_body_size;
    this->_auto_index = obj._auto_index;
    this->_cgi_lock = obj._cgi_lock;
    this->_upload_lock = obj._upload_lock;
    this->_redirect_lock = obj._redirect_lock;
    return *this;
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

void Location::setRedirectLocation(const std::string &redirectLocation) {
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

AutoIndexOption::option Location::getAutoIndexOption() const {
    return _auto_index;
}

void Location::setAutoIndex(bool autoIndex) {
    if (autoIndex)
        _auto_index = AutoIndexOption::ON;
    else
        _auto_index = AutoIndexOption::OFF;
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

long Location::getMaxBodySize() const {
    return _max_body_size;
}

void Location::setMaxBodySize(long maxBodySize) {
    _max_body_size = maxBodySize;
}

const std::string &Location::getDirectoryRequestPage() const {
    return _directory_request_page;
}

void Location::setDirectoryRequestPage(const std::string &directoryRequestPage) {
    _directory_request_page = directoryRequestPage;
}

const std::map<int, std::string> &Location::getErrorPages() const {
    return _error_pages;
}

void Location::setErrorPages(const std::map<int, std::string> &errorPages) {
    _error_pages = errorPages;
}


std::ostream &operator<<(std::ostream &os, const Location &location) {
    os
        << "\tPath: " << location.getPath() << std::endl
        << "\tRoot: " << location.getRoot() << std::endl
        << "\tIndex: " << location.getIndex() << std::endl
        << "\tRedirect Url: " << location.getRedirectLocation() << std::endl
        << "\tCgi Path: " << location.getCgiPath() << std::endl
        << "\tCgin Bin: " << location.getCgiBin() << std::endl
        << "\tUpload Path: " << location.getUploadPath() << std::endl
        << "\tAuto Index: " << location.getAutoIndexOption() << std::endl
        << "\tMax Body Size: " << location.getMaxBodySize() << std::endl
        << "\tCgi Lock: " << location.isCgiLock()
        << " | Upload Lock: " << location.isUploadLock()
        << " | Redirect Lock: " << location.isRedirectLock() << std::endl
        << "\tLimit Except: ";

    for (size_t i = 0; i < location.getLimitExcept().size(); i++) {
        os << location.getLimitExcept()[i] << " ";
        os.flush();
    }

    os << std::endl
       << "\tDirectory Page: " << location.getDirectoryRequestPage() << std::endl
       << "\tError Pages >" << std::endl;

    for (std::map<int, std::string>::const_iterator it = location.getErrorPages().begin();
         it != location.getErrorPages().end(); ++it) {
        os << "\t\t" << it->first << " Page: " << it->second << std::endl;
        os.flush();
    }
    return os;
}

Location::Location(const Location &obj) {
    *this = obj;
}

void Location::setCgiBin(const std::string &cgiBin) {
    _cgi_bin = cgiBin;
}

const std::string &Location::getCgiBin() const {
    return _cgi_bin;
}

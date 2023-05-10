//
// Created by rmore on 07/05/2023.
//

#include "../../includes/config/Location.h"

Location::Location() {
    this->_path = "/";
    this->_root = "./public";
    this->_limit_except = std::vector<std::string>();
    this->_limit_except.push_back("GET");
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

const std::string &Location::getRedirectUrl() const {
    return _redirect_url;
}

void Location::setRedirectUrl(const std::string &redirectUrl) {
    _redirect_url = redirectUrl;
}

const std::string &Location::getCgiPath() const {
    return _cgi_path;
}

void Location::setCgiPath(const std::string &cgiPath) {
    _cgi_path = cgiPath;
}

bool Location::isAutoindex() const {
    return _auto_index;
}

void Location::setAutoindex(bool autoindex) {
    Location::_auto_index = autoindex;
}

const std::string &Location::getUploadPath() const {
    return _upload_path;
}

void Location::setUploadPath(const std::string &uploadPath) {
    _upload_path = uploadPath;
}


std::ostream &operator<<(std::ostream &os, const Location &location) {
    os
        << "Location: " << std::endl
        << "Path: " << location.getPath() << std::endl
        << "Root: " << location.getRoot() << std::endl
        << "Redirect Url: " << location.getRedirectUrl() << std::endl
        << "Cgi Path: " << location.getCgiPath() << std::endl
        << "Upload Path: " << location.getUploadPath() << std::endl
        << "Autoindex: " << location.isAutoindex() << std::endl
        << "Limit Except: ";

    for (size_t i = 0; i < location.getLimitExcept().size(); i++) {
        os << location.getLimitExcept()[i] << " ";
        os.flush();
    }

    return os;
}
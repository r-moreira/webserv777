//
// Created by rmore on 15/03/2023.
//

#include "../../includes/network/Location.h"

Location::Location(){}

Location::~Location(){}

const std::string &Location::getRoot() const {
    return _root;
}

void Location::setRoot(const std::string &root) {
    _root = root;
}

const std::string &Location::getLocationNumber() const {
    return _locationNumber;
}

void Location::setLocationNumber(const std::string &locationNumber) {
    _locationNumber = locationNumber;
}

const std::string &Location::getLocationPath() const {
    return _locationPath;
}

void Location::setLocationPath(const std::string &locationPath) {
    _locationPath = locationPath;
}

const std::vector<std::string> &Location::getAllowedMethods() const {
    return _allowedMethods;
}

void Location::setAllowedMethods(const std::vector<std::string> &allowedMethods) {
    _allowedMethods = allowedMethods;
}

const std::string &Location::getClientMaxBodySize() const {
    return _clientMaxBodySize;
}

void Location::setClientMaxBodySize(const std::string &clientMaxBodySize) {
    _clientMaxBodySize = clientMaxBodySize;
}

const std::vector<std::string> &Location::getIndex() const {
    return _index;
}

void Location::setIndex(const std::vector<std::string> &index) {
    _index = index;
}

const std::string &Location::getAutoIndex() const {
    return _autoIndex;
}

void Location::setAutoIndex(const std::string &autoIndex) {
    _autoIndex = autoIndex;
}

const std::pair<std::string, std::string> &Location::getRedirection() const {
    return _redirection;
}

void Location::setRedirection(const std::pair<std::string, std::string> &redirection) {
    _redirection = redirection;
}

const std::string &Location::getUploadPath() const {
    return _uploadPath;
}

void Location::setUploadPath(const std::string &uploadPath) {
    _uploadPath = uploadPath;
}

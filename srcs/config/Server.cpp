//
// Created by rmore on 15/03/2023.
//

#include "../../includes/config/Server.h"

Server::Server() {
    this->_port = 8080;
    this->_index = "index.html";
    this->_root = "./";
    this->_fd = -1;
    this->_max_body_size = -1;
    this->_autoindex = false;
    this->_upload_lock = false;
    this->_limit_except = std::vector<std::string>();
    this->_limit_except.push_back("GET");
    this->_limit_except.push_back("POST");
    this->_limit_except.push_back("DELETE");
}

Server::~Server() {
}

const std::string &Server::getName() const {
    return _name;
}

void Server::setName(const std::string &name) {
    _name = name;
}

int Server::getPort() const {
    return _port;
}

void Server::setPort(int port) {
    _port = port;
}

int Server::getFd() const {
    return _fd;
}

void Server::setFd(int fd) {
    _fd = fd;
}

long Server::getMaxBodySize() const {
    return _max_body_size;
}

void Server::setMaxBodySize(long maxBodySize) {
    _max_body_size = maxBodySize;
}

const std::string &Server::getDirectoryRequestPage() const {
    return _directory_request_page;
}

void Server::setDirectoryRequestPage(const std::string &directoryRequestPage) {
    _directory_request_page = directoryRequestPage;
}

const std::map<int, std::string> &Server::getErrorPages() const {
    return _error_pages;
}

void Server::setErrorPages(const std::map<int, std::string> &errorPages) {
    _error_pages = errorPages;
}

const std::string &Server::getRoot() const {
    return _root;
}

void Server::setRoot(const std::string &root) {
    _root = root;
}

const std::string &Server::getIndex() const {
    return _index;
}

void Server::setIndex(const std::string &index) {
    _index = index;
}

const std::vector<Location> &Server::getLocations() const {
    return _locations;
}

void Server::setLocations(const std::vector<Location> &locations) {
    _locations = locations;
}

bool Server::isAutoindex() const {
    return _autoindex;
}

void Server::setAutoindex(bool autoindex) {
    _autoindex = autoindex;
}

const std::vector<std::string> &Server::getLimitExcept() const {
    return _limit_except;
}

void Server::setLimitExcept(const std::vector<std::string> &limitExcept) {
    _limit_except = limitExcept;
}

const std::string &Server::getUploadPath() const {
    return _upload_path;
}

void Server::setUploadPath(const std::string &uploadPath) {
    _upload_path = uploadPath;
}

bool Server::isUploadLock() const {
    return _upload_lock;
}

void Server::setUploadLock(bool uploadLock) {
    _upload_lock = uploadLock;
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
    os
       << "Name: " << server.getName() << std::endl
       << "Port: " << server.getPort() << std::endl
       << "Root: " << server.getRoot() << std::endl
       << "Index: " << server.getIndex() << std::endl
       << "Auto Index: " << server.isAutoindex() << std::endl
       << "Max Body Size: " << server.getMaxBodySize() << std::endl
       << "File Descriptor: " << server.getFd() << std::endl
       << "Upload Path: " << server.getUploadPath() << std::endl
       << "Upload Lock: " << server.isUploadLock() << std::endl
       << "Limit Except: ";

    for (size_t i = 0; i < server.getLimitExcept().size(); i++) {
        os << server.getLimitExcept()[i] << " ";
        os.flush();
    }

    os << std::endl
       << "Directory Page: " << server.getDirectoryRequestPage() << std::endl
       << "Error Pages >" << std::endl;

    for (std::map<int, std::string>::const_iterator it = server.getErrorPages().begin();
         it != server.getErrorPages().end(); ++it) {
        os << "\t" << it->first << " Page: " << it->second << std::endl;
        os.flush();
    }

    int i = 0;
    for (std::vector< Location>::const_iterator it = server.getLocations().begin();
         it != server.getLocations().end(); ++it) {
        os << "Location " << ++i << " >" << std::endl << *it << std::endl;
        os.flush();
    }
    return os;

}

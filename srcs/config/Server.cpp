//
// Created by rmore on 15/03/2023.
//

#include "../../includes/config/Server.h"

Server::Server(ConfigParser::ServerType *serverParam) {
    _name = serverParam->serverName;
    _port = serverParam->port;
    _root = serverParam->root;
    _index = serverParam->index[0];
    _max_body_size = serverParam->maxBodySize;
    _upload_path = serverParam->uploadPath;
    _directory_request_page = serverParam->directoryPage;
    // _error_pages = serverParam->errorPage;
    for (size_t i = 0; i < serverParam->errorPage.size(); i++) {
        int key = std::atoi(serverParam->errorPage[i].c_str());
        i += 1;
        _error_pages.insert(std::pair<int, std::string>(key, serverParam->errorPage[i]));
    }
    _limit_except = serverParam->limitExcept;
    _autoindex = serverParam->auto_index;
    _upload_lock = serverParam->uploadLock;
    for (size_t i = 0; i < serverParam->locations.size(); i++) {
        _locations.push_back(serverParam->locations[i]->getLocation());
    }
}

Server::Server() {}

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

void Server::setAutoindex(bool autoindex) {
    if (autoindex)
        this->_autoindex = AutoIndexOption::ON;
    else
        this->_autoindex = AutoIndexOption::OFF;
}

Server::AutoIndexOption::auto_index_option Server::getAutoIndexOption() const {
    return _autoindex;
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
    os
       << "Name: " << server.getName() << std::endl
       << "Port: " << server.getPort() << std::endl
       << "Root: " << server.getRoot() << std::endl
       << "Index: " << server.getIndex() << std::endl
       << "Auto Index: " << server.getAutoIndexOption() << std::endl
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

//
// Created by rmore on 15/03/2023.
//

#include "../../includes/config/Server.h"

Server::Server() {
    this->_name = "webserv";
    this->_port = 80;
    this->_fd = -1;
    this->_max_body_size = -1;

    Location location = Location();
    this->_locations.insert(std::pair<std::string, Location>(location.getPath(), location));
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

const std::map<std::string, Location> &Server::getLocations() const {
    return _locations;
}

void Server::setLocations(const std::map<std::string, Location> &locations) {
    _locations = locations;
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


//TODO: Printar todos os locations
std::ostream &operator<<(std::ostream &os, const Server &server) {

    os << "####### Server ####### " << std::endl
       << "Name: " << server.getName() << std::endl
       << "Port: " << server.getPort() << std::endl
       << "Max Body Size: " << server.getMaxBodySize() << std::endl
       << "Server FD: " << server.getFd() << std::endl
       << "Directory Request Page: " << server.getDirectoryRequestPage() << std::endl;

    os << "Error Pages >" << std::endl;
    for (std::map<int, std::string>::const_iterator it = server.getErrorPages().begin();
         it != server.getErrorPages().end(); ++it) {
        os << "\t" << it->first << " Page: " << it->second << std::endl;
        os.flush();
    }

    os << "####### Locations #######" << std::endl;
    for (std::map<std::string, Location>::const_iterator it = server.getLocations().begin();
         it != server.getLocations().end(); ++it) {
        os << it->second << std::endl;
        os.flush();
    }
    return os;

}

//
// Created by rmore on 15/03/2023.
//

#include "../../includes/config/Server.hpp"

Server& Server::build(ConfigParser::ServerType *serverParam) {
    _fd = -1;
    if (!serverParam->serverName.empty()) {
        _name = serverParam->serverName;
        addServerNametoHost(_name);
    }
    _port = serverParam->port;
    if (!serverParam->root.empty()) {
        _root = serverParam->root[0];
    }
    if(!serverParam->index.empty())
        _index = serverParam->index[0];
    _max_body_size = serverParam->maxBodySize;
    _upload_path = serverParam->uploadPath;
    _directory_request_page = serverParam->directoryPage;
    for (size_t i = 0; i < serverParam->errorPage.size(); i++) {
        int key = std::atoi(serverParam->errorPage[i].c_str());
        i += 1;
        _error_pages.insert(std::pair<int, std::string>(key, serverParam->errorPage[i]));
    }
    if (!serverParam->limitExcept.empty()) {
        _limit_except = serverParam->limitExcept;
    }
    _autoindex = serverParam->auto_index;
    _upload_lock = serverParam->uploadLock;
    for (size_t i = 0; i < serverParam->locations.size(); i++) {
        _locations.push_back(serverParam->locations[i]->getLocation());
    }
    return *this;
}

Server::Server() {
    _port = 8080;
    _index = "index.html";
    _root = "./";
    _fd = -1;
    _max_body_size = -1;
    _autoindex = AutoIndexOption::NONE;
    _upload_lock = false;
    _limit_except = std::vector<std::string>();
    _limit_except.push_back("GET");
    _limit_except.push_back("POST");
    _limit_except.push_back("DELETE");
}

Server::~Server() {}

const std::string &Server::getName() const {
    return _name;
}

void Server::setName(const std::string &name) {
    _name = name;
}

int Server::fileToString(const std::string& filename, std::string& fileContents) {
		std::ifstream input(filename.c_str());
		if (!input) {
			return (-1);
		}
		std::stringstream buffer;
		buffer << input.rdbuf();
		fileContents = buffer.str();
		return (1);
}

void Server::addServerNametoHost(std::string name) {
		std::string contents;
		std::string loopback = "127.0.0.1  "; 
		std::ofstream outfile("/etc/hosts", std::ios::app);

		if (!outfile) {
            Logger::warning("Could not open /etc/hosts to setup server_name: " + name + ". Try with sudo if you want to resolve server hosts");
			return;
		}
		if (fileToString("/etc/hosts", contents) == -1) {
            Logger::error("Could not capture contents of /etc/hosts file");
			return;
		}
        if(contents.find(loopback + name) == std::string::npos) {
            outfile << loopback << name << "\n";
        }
		outfile.close();
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

AutoIndexOption::option Server::getAutoIndexOption() const {
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

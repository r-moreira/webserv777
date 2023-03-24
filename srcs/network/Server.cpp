//
// Created by rmore on 15/03/2023.
//

#include "../../includes/network/Server.h"

Server::Server()
{
    _port = 80;
    en_handle = 0;
    a = 9000;
    init_MimeTypes();
}

Server::~Server()
{
}


std::vector<std::string> &Server::getmime_types()
{
    return _mimeTypes;
}



void Server::init_MimeTypes()
{
    std::ifstream file;
    file.open("resources/mime.types");
    std::string str;
    while(getline(file,str))
        _mimeTypes.push_back(str);
    file.close();
}

const Socket &Server::getSocket() const {
    return _socket;
}

void Server::setSocket(const Socket &socket) {
    _socket = socket;
}

size_t Server::getPort() const {
    return _port;
}

void Server::setPort(size_t port) {
    _port = port;
}

const std::vector<std::string> &Server::getServerNames() const {
    return _serverNames;
}

void Server::setServerNames(const std::vector<std::string> &serverNames) {
    _serverNames = serverNames;
}

const std::vector<Location> &Server::getLocations() const {
    return _locations;
}

void Server::setLocations(const std::vector<Location> &locations) {
    _locations = locations;
}

const std::vector<std::pair<std::string, std::string> > &Server::getErrorPages() const {
    return _errorPages;
}

void Server::setErrorPages(const std::vector<std::pair<std::string, std::string> > &errorPages) {
    _errorPages = errorPages;
}

const std::string &Server::getUploadPath() const {
    return _uploadPath;
}

void Server::setUploadPath(const std::string &uploadPath) {
    _uploadPath = uploadPath;
}

const std::vector<std::string> &Server::getMimeTypes() const {
    return _mimeTypes;
}

void Server::setMimeTypes(const std::vector<std::string> &mimeTypes) {
    _mimeTypes = mimeTypes;
}

const std::string &Server::getRoot() const {
    return root;
}

void Server::setRoot(const std::string &root) {
    Server::root = root;
}

char **Server::getEnv() const {
    return _env;
}

void Server::setEnv(char **env) {
    _env = env;
}

int Server::getA() const {
    return a;
}

void Server::setA(int a) {
    Server::a = a;
}

int Server::getEnHandle() const {
    return en_handle;
}

void Server::setEnHandle(int enHandle) {
    en_handle = enHandle;
}

const std::vector<std::pair<std::string, std::string> > &Server::getCgi() const {
    return _cgi;
}

void Server::setCgi(const std::vector<std::pair<std::string, std::string> > &cgi) {
    _cgi = cgi;
}



//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_SERVER_H
#define WEBSERV_SERVER_H

#include "../webserv.h"
#include "../network/Socket.h"
#include "Location.h"


class Server {

private:
    std::string _name;
    std::string _root;
    std::string _index;
    int _port;
    int _fd;
    long _max_body_size;
    std::string _directory_request_page;
    std::map<int, std::string> _error_pages;
    std::vector<Location> _locations;
public:
    Server();

    const std::vector<Location> &getLocations() const;

    void setLocations(const std::vector<Location> &locations);

    ~Server();

    const std::string &getName() const;

    void setName(const std::string &name);

    int getPort() const;

    void setPort(int port);

    int getFd() const;

    void setFd(int fd);

    long getMaxBodySize() const;

    void setMaxBodySize(long maxBodySize);

    const std::string &getDirectoryRequestPage() const;

    void setDirectoryRequestPage(const std::string &directoryRequestPage);

    const std::map<int, std::string> &getErrorPages() const;

    void setErrorPages(const std::map<int, std::string> &errorPages);

    const std::string &getRoot() const;

    void setRoot(const std::string &root);

    const std::string &getIndex() const;

    void setIndex(const std::string &index);
};

std::ostream& operator << (std::ostream& os, const Server& obj);

#endif //WEBSERV_SERVER_H

//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_SERVER_H
#define WEBSERV_SERVER_H

#include "../webserv.h"
#include "../network/Socket.h"
#include "Location.h"
#include "../parser/ConfigParser.hpp"

class Server {
public:
    struct AutoIndexOption {
        enum auto_index_option {
            OFF,
            ON,
            NONE = -1
        };
    };

private:
    std::string _name;
    std::string _root;
    std::string _index;
    int _port;
    int _fd;
    long _max_body_size;
    std::string _upload_path;
    std::string _directory_request_page;
    std::map<int, std::string> _error_pages;
    std::vector<std::string> _limit_except;
    std::vector<Location> _locations;
    AutoIndexOption::auto_index_option _autoindex;
    bool _upload_lock;
public:
    Server(ConfigParser::ServerType *serverParam);

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

    void setAutoindex(bool autoindex);

    AutoIndexOption::auto_index_option getAutoIndexOption() const;

    const std::vector<std::string> &getLimitExcept() const;

    void setLimitExcept(const std::vector<std::string> &limitExcept);

    const std::string &getUploadPath() const;

    void setUploadPath(const std::string &uploadPath);

    bool isUploadLock() const;

    void setUploadLock(bool uploadLock);
};

std::ostream& operator << (std::ostream& os, const Server& obj);

#endif //WEBSERV_SERVER_H

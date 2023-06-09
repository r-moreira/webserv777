//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "../webserv.hpp"
#include "../network/Socket.hpp"
#include "Location.hpp"
#include "../parser/ConfigParser.hpp"
#include "AutoIndexOption.hpp"
#include "../utils/Logger.hpp"

class ConfigParser;

class Server {
public:

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
    AutoIndexOption::option _autoindex;
    bool _upload_lock;

public:
    Server& build(ConfigParser::ServerType *serverParam);

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

    AutoIndexOption::option getAutoIndexOption() const;

    const std::vector<std::string> &getLimitExcept() const;

    void setLimitExcept(const std::vector<std::string> &limitExcept);

    const std::string &getUploadPath() const;

    void setUploadPath(const std::string &uploadPath);

    bool isUploadLock() const;

    void setUploadLock(bool uploadLock);

    void addServerNametoHost(std::string name);

    int fileToString(const std::string& filename, std::string& fileContents);
};

std::ostream& operator << (std::ostream& os, const Server& obj);

#endif //WEBSERV_SERVER_HPP

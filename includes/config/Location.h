//
// Created by rmore on 07/05/2023.
//

#ifndef WEBSERV_LOCATION_H
#define WEBSERV_LOCATION_H

#include "../webserv.h"

class Location {

private:
    std::string _path;
    std::string _root;
    std::string _index;
    std::vector<std::string> _limit_except;
    std::string _redirect_location;
    std::string _cgi_path;
    std::string _upload_path;
    std::string _directory_request_page;
    std::map<int, std::string> _error_pages;
    long _max_body_size;
    bool _auto_index;
    bool _cgi_lock;
    bool _upload_lock;
    bool _redirect_lock;

public:
    Location();

    const std::string &getPath() const;

    void setPath(const std::string &path);

    const std::string &getRoot() const;

    void setRoot(const std::string &root);

    const std::vector<std::string> &getLimitExcept() const;

    void setLimitExcept(const std::vector<std::string> &limitExcept);

    const std::string &getRedirectLocation() const;

    void setRedirectLocation(const std::string &redirectLocation);

    const std::string &getCgiPath() const;

    void setCgiPath(const std::string &cgiPath);

    const std::string &getUploadPath() const;

    void setUploadPath(const std::string &uploadPath);

    bool isAutoIndex() const;

    void setAutoIndex(bool autoIndex);

    bool isCgiLock() const;

    void setCgiLock(bool cgiLock);

    bool isUploadLock() const;

    void setUploadLock(bool uploadLock);

    bool isRedirectLock() const;

    void setRedirectLock(bool redirectLock);

    const std::string &getIndex() const;

    void setIndex(const std::string &index);

    long getMaxBodySize() const;

    void setMaxBodySize(long maxBodySize);

    const std::string &getDirectoryRequestPage() const;

    void setDirectoryRequestPage(const std::string &directoryRequestPage);

    const std::map<int, std::string> &getErrorPages() const;

    void setErrorPages(const std::map<int, std::string> &errorPages);
};

std::ostream& operator << (std::ostream& os, const Location& obj);

#endif //WEBSERV_LOCATION_H

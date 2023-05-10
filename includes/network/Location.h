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
    std::vector<std::string> _limit_except;
    std::string _redirect_url;
    std::string _cgi_path;
    std::string _upload_path;
    bool _auto_index;
public:
    Location();

    const std::string &getPath() const;

    void setPath(const std::string &path);

    const std::string &getRoot() const;

    void setRoot(const std::string &root);

    const std::vector<std::string> &getLimitExcept() const;

    void setLimitExcept(const std::vector<std::string> &limitExcept);

    bool isCgiLock() const;

    void setCgiLock(bool cgiLock);

    const std::string &getRedirectUrl() const;

    void setRedirectUrl(const std::string &redirectUrl);

    const std::string &getCgiPath() const;

    void setCgiPath(const std::string &cgiPath);

    bool isAutoindex() const;

    void setAutoindex(bool autoindex);

    const std::string &getUploadPath() const;

    void setUploadPath(const std::string &uploadPath);
};

std::ostream& operator << (std::ostream& os, const Location& obj);

#endif //WEBSERV_LOCATION_H

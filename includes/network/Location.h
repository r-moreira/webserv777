//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_LOCATION_H
#define WEBSERV_LOCATION_H

#include "../webserv.h"

class Location {
private:
    std::string _root;
    std::string _locationNumber;
    std::string _locationPath;
    std::vector<std::string> _allowedMethods;
    std::string _clientMaxBodySize;
    std::vector<std::string> _index;
    std::string _autoIndex;
    std::pair<std::string, std::string> _redirection;
    std::string _uploadPath;

public:
    Location();

    ~Location();

    const std::string &getRoot() const;

    void setRoot(const std::string &root);

    const std::string &getLocationNumber() const;

    void setLocationNumber(const std::string &locationNumber);

    const std::string &getLocationPath() const;

    void setLocationPath(const std::string &locationPath);

    const std::vector<std::string> &getAllowedMethods() const;

    void setAllowedMethods(const std::vector<std::string> &allowedMethods);

    const std::string &getClientMaxBodySize() const;

    void setClientMaxBodySize(const std::string &clientMaxBodySize);

    const std::vector<std::string> &getIndex() const;

    void setIndex(const std::vector<std::string> &index);

    const std::string &getAutoIndex() const;

    void setAutoIndex(const std::string &autoIndex);

    const std::pair<std::string, std::string> &getRedirection() const;

    void setRedirection(const std::pair<std::string, std::string> &redirection);

    const std::string &getUploadPath() const;

    void setUploadPath(const std::string &uploadPath);

};

#endif //WEBSERV_LOCATION_H

#ifndef Autoindex_hpp
#define Autoindex_hpp

#include <iostream>
#include <dirent.h>

#include "../config/Server.h"

class AutoIndex {
public:
    static std::string pageGenerator(const std::string& request_root, const std::string& request_uri, int port);

    AutoIndex();

private:
    static std::string _linkGenerator(const std::string& pathLink, const std::string& dirEntry, int port);
};

#endif
#ifndef Autoindex_hpp
#define Autoindex_hpp
#include <iostream>
#include <dirent.h>

#include "../network/Server.h"

class Autoindex
{    
public:
    std::string pageGenerator(const std::string target, const Server &serve);
    Autoindex();
private:
    std::string _linkGenerator(const std::string pathLink, std::string dirEntry, const Server &server);
};
#endif
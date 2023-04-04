#ifndef Http_hpp
#define Http_hpp
#include <iostream>

#include "../Read.hpp"

namespace FT {

class Http: public Read
{
public:
    Http(int socket);
    std::string getRequest();
    ~Http();
};
}

#endif
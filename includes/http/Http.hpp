#ifndef Http_hpp
#define Http_hpp
#include <iostream>

#include "../Read.hpp"
#include "HttpRequest.hpp"

namespace FT {

class Http: public Read
{
public:
    Http(int socket);
    HttpRequest *get_request();
    ~Http();
private:
    HttpRequest *request;
};
}

#endif
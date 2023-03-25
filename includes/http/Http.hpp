#ifndef Http_hpp
#define Http_hpp
#include <iostream>

#include "../Reading.hpp"
#include "HttpRequest.hpp"

namespace FT {

class Http: private Reading
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
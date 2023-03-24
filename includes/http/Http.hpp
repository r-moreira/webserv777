#ifndef Http_hpp
#define Http_hpp
#include <iostream>

#include "../Reading.hpp"

namespace FT {

class Http: public Reading
{
public:
    Http(int socket);
    ~Http();
};
}

#endif
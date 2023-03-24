#ifndef Server_hpp
#define Server_hpp

#include <unistd.h>
#include <stdio.h>

#include "ServerTemplate.hpp"
#include "../http/Http.hpp"

namespace FT {
class Server : ServerTemplate
{
private:
    FT::Http* http;
    int newSocket;
    void accepter();
    void handler();
    void responder();
public:
    Server();
    void launch();
};
}
#endif
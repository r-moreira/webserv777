#ifndef Server_hpp
#define Server_hpp

#include "ServerTemplate.hpp"
#include <unistd.h>
#include <stdio.h>

namespace FT {
class Server : ServerTemplate
{
private:
    char buffer[30000] = {0};
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
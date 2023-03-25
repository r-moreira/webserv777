#ifndef Server_hpp
#define Server_hpp

#include <unistd.h>
#include <stdio.h>

#include "ServerTemplate.hpp"
#include "../http/Http.hpp"

namespace FT {
class Server : ServerTemplate
{
public:
    Server();
    void launch();
private:
    FT::Http* http;
    int newSocket;
    void accepter();
    void handler();
    void responder();
};
}
#endif
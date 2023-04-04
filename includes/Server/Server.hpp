#ifndef Server_hpp
#define Server_hpp

#include <unistd.h>
#include <stdio.h>

#include "ServerTemplate.hpp"
#include "../MultPlexing/MultPlexing.hpp"

namespace FT {
class Server : ServerTemplate, Multplexing
{
public:
    Server();
    void launch();
private:
    int newSocket;
    void accepter();
};
}
#endif
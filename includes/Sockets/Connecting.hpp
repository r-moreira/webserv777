#ifndef Connecting_hpp
#define Connecting_hpp
#include <iostream>

#include "Socket.hpp"
namespace FT {
class Connecting: public Socket
{
public:
    Connecting(int domain, int service, int protocol, int port, u_long interface);

private:
    void connect_to_network(int sock, struct sockaddr_in address);
    int connection;
};
}
#endif
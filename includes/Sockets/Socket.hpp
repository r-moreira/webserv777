#ifndef Socket_hpp
#define Socket_hpp
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

namespace FT {
class Socket
{

public:
    Socket(int domain, int type, int protocol, int port, u_long interface);
    virtual void connect_to_network(int sock, struct sockaddr_in address) = 0;
    void is_connected(int network);
    int get_socket();
    struct sockaddr_in get_address();

private:
    struct sockaddr_in address;
    int sock;
    int connection;
};
}
#endif
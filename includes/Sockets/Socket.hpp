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
    virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
    void is_connected(int network);
    int get_socket();
    int get_connection();
    struct sockaddr_in get_address();
    void set_connection(int newConnection);

private:
    struct sockaddr_in address;
    int sock;
    int connection;
};
}
#endif
#ifndef BindingSocket_hpp
#define BindingSocket_hpp

#include <iostream>

#include "Socket.hpp"

namespace FT {
class BindingSocket: public Socket {

public:
    BindingSocket(int domain, int type, int protocol, int port, u_long interface);
    int connect_to_network(int sock, struct sockaddr_in address);

};
}
#endif
#ifndef ListeningSocket_hpp
#define ListeningSocket_hpp
#include <iostream>
#include <fcntl.h>

#include "BindingSocket.hpp"

namespace FT {
class ListeningSocket: public BindingSocket
{
public:
    ListeningSocket(int domain, int type, int protocol, int port, u_long interface, int bklg);
    void start_listening();
    int socket_accepter();
private:
    void set_non_blocking(int fd);
    int backlog;
    int listening;
};
}
#endif
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
private:
    void set_non_blocking();
    int backlog;
    int listening;
};
}
#endif
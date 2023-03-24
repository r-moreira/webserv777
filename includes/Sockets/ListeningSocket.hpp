#ifndef ListeningSocket_hpp
#define ListeningSocket_hpp
#include <iostream>

#include "BindingSocket.hpp"

namespace FT {
class ListeningSocket: public BindingSocket
{
public:
    ListeningSocket(int domain, int type, int protocol, int port, u_long interface, int bklg);
    void start_listening();
private:
    int backlog;
    int listening;
};
}
#endif
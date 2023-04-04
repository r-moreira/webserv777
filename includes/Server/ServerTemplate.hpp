#ifndef ServerTemplate_hpp
#define ServerTemplate_hpp

#include "../Sockets/hdelibc-sockets.hpp"

namespace FT {
class ServerTemplate
{
public:
    ServerTemplate(int domain, int type, int protocol, int port, u_long interface, int bklg);
    ~ServerTemplate();
    virtual void launch() = 0;
    ListeningSocket *get_socket();
private:
    ListeningSocket *socket;
    virtual void accepter() = 0;
};
}
#endif
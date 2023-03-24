#include "../../includes/Server/ServerTemplate.hpp"

FT::ListeningSocket* FT::ServerTemplate::get_socket() {
    return socket;
}

FT::ServerTemplate::ServerTemplate(int domain, int type,
    int protocol, int port, u_long interface, int bklg)
{
    socket = new ListeningSocket(domain, type, protocol, port, interface, bklg);
}
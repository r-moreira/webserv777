#include "../../includes/Sockets/ListeningSocket.hpp"

FT::ListeningSocket::ListeningSocket(int domain, int type, int protocol,
    int port, u_long interface, int bklg) : BindingSocket(domain, type, protocol, port, interface)
{
    backlog = bklg;
    start_listening();
    is_connected(listening);
}

void FT::ListeningSocket::start_listening(void) {
    listening = listen(get_socket(), backlog);
}
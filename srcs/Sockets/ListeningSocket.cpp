#include "../../includes/Sockets/ListeningSocket.hpp"

FT::ListeningSocket::ListeningSocket(int domain, int type, int protocol,
    int port, u_long interface, int bklg) : BindingSocket(domain, type, protocol, port, interface)
{
    backlog = bklg;
    start_listening();
    is_connected(listening);
    set_non_blocking();
}

void FT::ListeningSocket::start_listening(void) {
    listening = listen(get_socket(), backlog);
}

void FT::ListeningSocket::set_non_blocking() {
    int flags = fcntl(get_socket(), F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Error while getting flags: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK;
    if (fcntl(get_socket(), F_SETFL, flags) == -1) {
        std::cerr << "Error while setting flags: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
}

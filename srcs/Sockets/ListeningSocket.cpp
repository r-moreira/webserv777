#include "../../includes/Sockets/ListeningSocket.hpp"

FT::ListeningSocket::ListeningSocket(int domain, int type, int protocol,
    int port, u_long interface, int bklg) : BindingSocket(domain, type, protocol, port, interface)
{
    backlog = bklg;
    start_listening();
    is_connected(listening);
    set_non_blocking(get_socket());
}

void FT::ListeningSocket::start_listening(void) {
    listening = listen(get_socket(), backlog);
}

void FT::ListeningSocket::set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Error while getting flags: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        std::cerr << "Error while setting flags: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
}

int FT::ListeningSocket::socket_accepter() {
    struct sockaddr_in address = get_address();
    int addrlen = sizeof(address);
    int fd = accept(get_socket(), (struct sockaddr *)&address, (socklen_t*)&addrlen);
    set_non_blocking(fd);
    return fd;
}

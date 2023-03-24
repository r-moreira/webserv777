#include "../../includes/Sockets/Socket.hpp"

FT::Socket::Socket(int domain, int type, int protocol, int port, u_long interface) {
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    sock = socket(domain, type, protocol);
    is_connected(sock);
}

void FT::Socket::is_connected(int network) {

    if (network < 0) {
        perror("Failed to connect...");
        exit(EXIT_FAILURE);
    }
}

int FT::Socket::get_socket(void) {
    return sock;
}

struct sockaddr_in  FT::Socket::get_address(void) {
    return address;
}

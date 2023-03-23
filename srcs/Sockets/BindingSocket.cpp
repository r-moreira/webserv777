#include "../../includes/Sockets/BindingSocket.hpp"

FT::BindingSocket::BindingSocket(int domain, int type, int protocol,
    int port, u_long interface): Socket(domain, type, protocol, port, interface)
{
    connect_to_network(get_socket(), get_address());
    is_connected(binding);
}

void FT::BindingSocket::connect_to_network(int sock, struct sockaddr_in address) {
    binding = bind(sock, (struct sockaddr *)&address, sizeof(address));
}
#include "../../includes/Sockets/Connecting.hpp"

FT::Connecting::Connecting(int domain, int service, int protocol,
    int port, u_long interface): Socket(domain, service, protocol,port, interface)
{
    set_connection(connect_to_network(get_socket(), get_address()));
    is_connected(get_connection());
}

int FT::Connecting::connect_to_network(int sock, struct sockaddr_in address) {
    return connect(sock, (struct sockaddr *)&address, sizeof(address));
}
#include "../../includes/Server/Server.hpp"
#include <cstring>

FT::Server::Server(): ServerTemplate(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 100), Multplexing()
{
    launch();
}

void FT::Server::accepter() {
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    newSocket = accept(get_socket()->get_socket(),
        (struct sockaddr *)&address, (socklen_t*)&addrlen);
    newRequest(newSocket);
}

void FT::Server::launch() {
    while (1)
    {
        std::cout << "================ LISTENING ================" << std::endl;
        accepter();
        std::cout << "================ DONE ================" << std::endl;
    }
}
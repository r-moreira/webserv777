#include "../../includes/Server/Server.hpp"
#include <cstring>

FT::Server::Server(): ServerTemplate(AF_INET, SOCK_STREAM, 0, 8082, INADDR_ANY, 10)
{
    launch();
}

void FT::Server::accepter() {
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    newSocket = accept(get_socket()->get_socket(),
        (struct sockaddr *)&address, (socklen_t*)&addrlen);
    http = new Http(newSocket);
}

void FT::Server::handler() {
    std::cout << http->get_text() << std::endl;
}

void FT::Server::responder() {
    char *str = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    write(newSocket , str , strlen(str));
    close(newSocket);
}

void FT::Server::launch() {
    while (1)
    {
        std::cout << "================ LISTENING ================" << std::endl;
        accepter();
        handler();
        responder();
        std::cout << "================ DONE ================" << std::endl;
    }
}
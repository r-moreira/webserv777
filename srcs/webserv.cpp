#include "../includes/webserv.h"
#include "../includes/io/Multiplexer.h"
#include "../includes/network/ServerBuilder.h"
#include "../includes/ConfigParcer/ConfigParcer.hpp"

int main(int argc, char **argv, char **env) {
    signal(SIGPIPE, SIG_IGN);

    if (argc != 2) {
        std::cout << CYAN << "Usage :  ./webserv {PATH TO CONFIGURATION FILE}" << RESET << std::endl;
        return EXIT_FAILURE;
    }
    char tmp[256];
    getcwd(tmp, 256);
    std::cout << "Current working directory: " << tmp << std::endl;

    int port = 8080;

    Server server = Server::build()
            .with_name("webserv")
            .with_port(port)
            .start();

    Server server2 = Server::build()
            .with_name("webserv2")
            .with_port(port + 1)
            .start();

    std::vector<Server> servers;
    servers.insert(servers.begin(), server);
    servers.insert(servers.begin() + 1, server2);

    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
        std::cout << BLUE << *it << RESET << std::endl;
    }

    Multiplexer multiplexer(servers);
    multiplexer.event_loop();

    return EXIT_SUCCESS;
}
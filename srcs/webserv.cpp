#include "../includes/webserv.h"
#include "../includes/io/Multiplexer.h"
#include "../includes/network/Server.h"
#include "../includes/network/ServerBuilder.h"

int main(int argc, char **argv, char **env) {
    signal(SIGPIPE, SIG_IGN);

    if (argc != 2) {
        std::cout << CYAN << "Usage :  ./webserv {PATH TO CONFIGURATION FILE}" << RESET << std::endl;
        return EXIT_FAILURE;
    }

    srand(time(NULL));
    int port = 8080 + (rand() % 10);

    Server server = Server::build()
            .with_name("webserv")
            .with_port(port)
            .start();

    std::cout << BLUE << server << RESET << std::endl;

    std::vector<Server> servers;
    servers.insert(servers.begin(), server);

    Multiplexer multiplexer(servers);
    multiplexer.event_loop();

    return EXIT_SUCCESS;
}
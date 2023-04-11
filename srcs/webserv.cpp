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

    //TODO: Fazer a classe multiplexer receber uma lista da classe Server

    int port = 8080 + (rand() % 10);

    Multiplexer multiplexer;
    Server server = Server::build()
            .with_name("webserv")
            .with_port(port)
            .start();

    std::cout << BLUE << server << RESET << std::endl;

    multiplexer.event_loop(server.getFd());

    return EXIT_SUCCESS;
}
#include "../includes/webserv.h"
#include "../includes/io/Multiplexer.h"

int main(int argc, char **argv, char **env) {
    signal(SIGPIPE, SIG_IGN);

    if (argc != 2) {
        std::cout << CYAN << "Usage :  ./webserv {PATH TO CONFIGURATION FILE}" << RESET << std::endl;
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    //TODO: Fazer a classe multiplexer receber uma lista da classe Server
    Multiplexer multiplexer;
    multiplexer.event_loop(Socket::setupServer(8080 + (rand() % 10)));

    return EXIT_SUCCESS;
}
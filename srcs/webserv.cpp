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

    std::vector<Server> servers;
    FT::ConfigParcer conf = FT::ConfigParcer(argv[1]);
    for (int i = 0; i < conf.getHowMuchServers(); i++) {
        Server server = Server::build()
                .with_name(conf[i].serverName)
                .with_port(conf[i].port)
                .start();
        
        servers.push_back(server);
    }
    
    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
        std::cout << BLUE << *it << RESET << std::endl;
    }


    Multiplexer multiplexer(servers);
    multiplexer.event_loop();

    return EXIT_SUCCESS;
}
#include "../includes/webserv.h"
#include "../includes/io/Multiplexer.h"
#include "../includes/parser/ConfigParser.h"
#include "../includes/utils/Logger.h"

void start_servers(std::vector<Server> &servers);
std::vector<Server> servers_builder(std::vector<ConfigParser::ServerType *> serverConfigList);

int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);

    (void) argv;

    if (argc != 2) {
        Logger::warning("Usage :  ./webserv {PATH TO CONFIGURATION FILE}");
        return EXIT_FAILURE;
    }

    ConfigParser configParser(argv[1]);

    std::vector<Server> servers = servers_builder(configParser.getServers());

    start_servers(servers);

    Multiplexer multiplexer(servers);
    multiplexer.event_loop();

    return EXIT_SUCCESS;
}

void start_servers(std::vector<Server> &servers) {
    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
        sleep(1);
        it->setFd(Socket::setupServer(it->getPort()));
        std::ostringstream log;
        log << "Server config:" << std::endl << BLUE << *it << RESET << std::endl << std::flush;
        Logger::debug(log.str());
    }
}

std::vector<Server> servers_builder(std::vector<ConfigParser::ServerType *> serverConfigList) {
    std::vector<Server> servers;
    for (size_t i = 0; i < serverConfigList.size(); i++) {
        servers.push_back(Server().build(serverConfigList[i]));
    }
    return servers;
}
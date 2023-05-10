#include "../includes/webserv.h"
#include "../includes/io/Multiplexer.h"
#include "../includes/parser/ConfigParser.hpp"

int main(int argc, char **argv, char **env) {
    signal(SIGPIPE, SIG_IGN);

    if (argc != 2) {
        std::cout << CYAN << "Usage :  ./webserv {PATH TO CONFIGURATION FILE}" << RESET << std::endl;
        return EXIT_FAILURE;
    }
    char tmp[256];
    getcwd(tmp, 256);
    std::cout << "Current working directory: " << tmp << std::endl;

    srand(time(0));
    int port = 8080 + rand() % 10;

    Location location = Location();
    location.setPath("/puppy");
    location.setRoot("./public");

    Server server = Server();

    std::map<std::string, Location> locations = server.getLocations();
    locations.clear();
    locations.insert(std::pair<std::string, Location>(location.getPath(), location));

    server.setName("webserv");
    server.setPort(port);
    server.setLocations(locations);



    Server server2 = Server();
    server2.setName("webserv2");
    server2.setPort(port + 1);
    server2.setMaxBodySize(10);

    std::vector<std::string> allowed_methods_get_post;
    allowed_methods_get_post.push_back("GET");
    allowed_methods_get_post.push_back("POST");

    Location location2 = Location();
    location2.setPath("/");
    location2.setRoot("./public");
    location2.setLimitExcept(allowed_methods_get_post);

    std::map<std::string, Location> locations2 = server2.getLocations();
    locations2.clear();
    locations2.insert(std::pair<std::string, Location>(location2.getPath(), location2));

    server2.setLocations(locations2);

    std::vector<Server> servers;
    servers.insert(servers.begin(), server);
    servers.insert(servers.begin() + 1, server2);

    //Temporário, apenas para ver as configs
    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
        sleep(1);
        it->setFd(Socket::setupServer(it->getPort()));
        std::cout << BLUE << *it << RESET << std::endl << std::flush;
    }

    Multiplexer multiplexer(servers);
    multiplexer.event_loop();

    return EXIT_SUCCESS;
}
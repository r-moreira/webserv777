#include "../includes/webserv.h"
#include "../includes/io/Multiplexer.h"
#include "../includes/parser/ConfigParser.hpp"

Server build_server_one(int port);

Server build_server_two(int port);

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

    Server server = build_server_one(port);
    Server server2 = build_server_two(port);

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

Server build_server_one(int port) {
    Location location = Location();
    location.setPath("/puppy");
    location.setRoot("./public/website");

    Server server = Server();

    std::map<std::string, Location> locations = server.getLocations();
    locations.clear();
    locations.insert(std::pair<std::string, Location>(location.getPath(), location));

    std::map<int, std::string> error_pages;

    error_pages.insert(std::pair<int, std::string>(404, "./public/error-pages/404.html"));
    error_pages.insert(std::pair<int, std::string>(413, "./public/error-pages/413.html"));

    server.setName("webserv");
    server.setPort(port);
    server.setLocations(locations);
    server.setErrorPages(error_pages);
    server.setDirectoryRequestPage("./public/directory-page/index.html");

    return server;
}

Server build_server_two(int port) {
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
    return server2;
}

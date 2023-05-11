#include "../includes/webserv.h"
#include "../includes/io/Multiplexer.h"
#include "../includes/parser/ConfigParser.hpp"

Server build_server_one(int port);

Server build_server_two(int port);

Server build_server_three(int port);

int main(int argc, char **argv, char **env) {
    signal(SIGPIPE, SIG_IGN);

    if (argc != 2) {
        std::cout << CYAN << "Usage :  ./webserv {PATH TO CONFIGURATION FILE}" << RESET << std::endl;
        return EXIT_FAILURE;
    }

    srand(time(0));
    int port = 8080 + rand() % 10;
    Server server = build_server_one(port);
    Server server2 = build_server_two(port + 1);
    Server server3 = build_server_three(port + 2);

    std::vector<Server> servers;
    servers.insert(servers.begin(), server);
    servers.insert(servers.begin() + 1, server2);
    servers.insert(servers.begin() + 2, server3);

    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
        sleep(1);
        it->setFd(Socket::setupServer(it->getPort()));
        std::cout << BLUE << *it << RESET << std::endl << std::flush;
    }

    Multiplexer multiplexer(servers);
    multiplexer.event_loop();

    return EXIT_SUCCESS;
}

Server build_server_three(int port) {
    Server server = Server();

    std::vector<Location> locations = server.getLocations();
    locations.clear();

    std::vector<std::string> allowed_method_get;
    allowed_method_get.push_back("GET");

    Location website_location = Location();
    website_location.setPath("/");
    website_location.setRoot("./public/website");
    website_location.setIndex("index.html");

    Location website_location2 = Location();
    website_location2.setPath("/puppy");
    website_location2.setRoot("./public/website");
    website_location.setIndex("index.html");

    Location redirect_location = Location();
    redirect_location.setRedirectLock(true);
    redirect_location.setPath("/redirect");
    redirect_location.setRedirectUrl("https://google.com");
    redirect_location.setLimitExcept(allowed_method_get);

    locations.push_back(website_location);
    locations.push_back(website_location2);
    locations.push_back(redirect_location);

    std::map<int, std::string> error_pages;
    error_pages.insert(std::pair<int, std::string>(404, "./public/error-pages/404.html"));
    error_pages.insert(std::pair<int, std::string>(405, "./public/error-pages/413.html"));
    error_pages.insert(std::pair<int, std::string>(413, "./public/error-pages/413.html"));

    server.setName("webserv3");
    server.setPort(port);
    server.setLocations(locations);
    server.setErrorPages(error_pages);
    return server;
}

Server build_server_two(int port) {
    Server server = Server();
    std::vector<Location> locations = server.getLocations();
    locations.clear();

    std::vector<std::string> allowed_methods_get_post;
    allowed_methods_get_post.push_back("GET");
    allowed_methods_get_post.push_back("POST");

    Location cgi_location = Location();
    cgi_location.setCgiLock(true);
    cgi_location.setPath("/cgi");
    cgi_location.setCgiPath("./public/cgi/hello.py");
    cgi_location.setLimitExcept(allowed_methods_get_post);


    std::vector<std::string> allowed_method_post;
    allowed_method_post.push_back("POST");

    Location upload_location = Location();
    upload_location.setUploadLock(true);
    upload_location.setPath("/upload");
    upload_location.setLimitExcept(allowed_method_post);


    std::vector<std::string> allowed_method_get;
    allowed_method_get.push_back("GET");

    Location static_location = Location();
    static_location.setPath("/images");
    static_location.setRoot("./public/static");
    static_location.setLimitExcept(allowed_method_get);

    locations.push_back(cgi_location);
    locations.push_back(upload_location);
    locations.push_back(static_location);

    server.setPort(port);
    server.setDirectoryRequestPage("./public/directory-page/index.html");
    server.setLocations(locations);
    return server;
}

Server build_server_one(int port) {
    Server server = Server();
    std::vector<Location> locations = server.getLocations();
    locations.clear();

    Location hello_world_location = Location();
    hello_world_location.setPath("/hello");

    locations.push_back(hello_world_location);

    std::map<int, std::string> error_pages;
    error_pages.insert(std::pair<int, std::string>(500, "./public/error-pages/500.html"));

    server.setName("webserv");
    server.setIndex("hello.html");
    server.setRoot("./public/hello-world");
    server.setPort(port);
    server.setMaxBodySize(10);
    server.setErrorPages(error_pages);
    server.setLocations(locations);
    return server;
}

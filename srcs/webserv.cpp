#include "../includes/webserv.h"
#include "../includes/io/Multiplexer.h"
#include "../includes/parser/ConfigParser.hpp"

Server build_server_one(int port);

Server build_server_two(int port);

Server build_server_three(int port);

Server build_default_server(int port);

std::vector<Server> servers_builder();

void start_servers(std::vector<Server> &servers);

int main(int argc, char **argv, char **env) {
    signal(SIGPIPE, SIG_IGN);

    if (argc != 2) {
        std::cout << CYAN << "Usage :  ./webserv {PATH TO CONFIGURATION FILE}" << RESET << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<Server> servers = servers_builder();
    start_servers(servers);

    Multiplexer multiplexer(servers);
    multiplexer.event_loop();

    return EXIT_SUCCESS;
}

void start_servers(std::vector<Server> &servers) {
    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
        sleep(1);
        it->setFd(Socket::setupServer(it->getPort()));
        std::cout << BLUE << *it << RESET << std::endl << std::flush;
    }
}

std::vector<Server> servers_builder() {
    //Rand apenas para evitar conflito de porta.
    //srand(time(0));
    //int port = 8080 + rand() % 10;
    int port = 8080;

    Server default_server = build_default_server(port);
    Server server = build_server_one(port + 1);
    Server server2 = build_server_two(port + 2);
    Server server3 = build_server_three(port + 3);

    std::vector<Server> servers;
    servers.insert(servers.begin(), default_server);
    servers.insert(servers.begin() + 1, server);
    servers.insert(servers.begin() + 2, server2);
    servers.insert(servers.begin() + 3, server3);
    return servers;
}

Server build_default_server(int port) {
    Server server = Server();

    // Não precisa setar nem a porta, por default será 8080, apenas para evitar conflito de porta.
    server.setPort(port);
    return server;
}

Server build_server_three(int port) {
    Server server = Server();
    std::vector<Location> locations = server.getLocations();

    std::vector<std::string> allowed_method_get;
    allowed_method_get.push_back("GET");

    Location puppy_care_location_one = Location();
    puppy_care_location_one.setPath("/");
    puppy_care_location_one.setRoot("./public/puppy-care");
    puppy_care_location_one.setIndex("index.html");
    puppy_care_location_one.setLimitExcept(allowed_method_get);

    Location puppy_care_location_two = Location();
    puppy_care_location_two.setPath("/care");
    puppy_care_location_two.setRoot("./public/puppy-care");
    puppy_care_location_one.setIndex("index.html");
    puppy_care_location_one.setLimitExcept(allowed_method_get);

    std::map<int, std::string> error_pages_lover_location;
    error_pages_lover_location.insert(std::pair<int, std::string>(403, "./public/error-pages/403.html"));
    error_pages_lover_location.insert(std::pair<int, std::string>(500, "./public/error-pages/500.html"));

    Location pet_lover_location = Location();
    pet_lover_location.setPath("/lover");
    pet_lover_location.setRoot("./public/pet-lover");
    pet_lover_location.setIndex("index.html");
    pet_lover_location.setLimitExcept(allowed_method_get);
    pet_lover_location.setErrorPages(error_pages_lover_location);

    Location redirect_location = Location();
    redirect_location.setRedirectLock(true);
    redirect_location.setPath("/redirect");
    redirect_location.setRedirectLocation("https://google.com");
    redirect_location.setLimitExcept(allowed_method_get);

    locations.push_back(puppy_care_location_one);
    locations.push_back(puppy_care_location_two);
    locations.push_back(pet_lover_location);
    locations.push_back(redirect_location);

    std::map<int, std::string> error_pages_server;
    error_pages_server.insert(std::pair<int, std::string>(404, "./public/error-pages/404.html"));
    error_pages_server.insert(std::pair<int, std::string>(405, "./public/error-pages/405.html"));
    error_pages_server.insert(std::pair<int, std::string>(413, "./public/error-pages/413.html"));

    server.setName("puppystore.com");
    server.setPort(port);
    server.setLocations(locations);
    server.setDirectoryRequestPage("./public/directory-page/index.html");
    server.setErrorPages(error_pages_server);
    return server;
}

Server build_server_two(int port) {
    Server server = Server();
    std::vector<Location> locations = server.getLocations();

    std::vector<std::string> allowed_methods_get_post;
    allowed_methods_get_post.push_back("GET");
    allowed_methods_get_post.push_back("POST");

    Location cgi_location = Location();
    cgi_location.setCgiLock(true);
    cgi_location.setPath("/cgi");
    cgi_location.setCgiPath("./public/cgi/hello.py");
    cgi_location.setLimitExcept(allowed_methods_get_post);


    std::vector<std::string> allowed_method_post_delete;
    allowed_method_post_delete.push_back("POST");
    allowed_method_post_delete.push_back("DELETE");

    Location upload_location = Location();
    upload_location.setUploadLock(true);
    upload_location.setPath("/upload");
    upload_location.setRoot("./public/uploaded-files/alternative");
    upload_location.setUploadPath("./public/uploaded-files/alternative");
    upload_location.setLimitExcept(allowed_method_post_delete);

    std::vector<std::string> allowed_method_get;
    allowed_method_get.push_back("GET");

    Location static_location = Location();
    upload_location.setAutoIndex(true);
    static_location.setPath("/images");
    static_location.setRoot("./public/images");
    static_location.setLimitExcept(allowed_method_get);

    locations.push_back(cgi_location);
    locations.push_back(upload_location);
    locations.push_back(static_location);

    server.setPort(port);
    server.setUploadPath("./public/uploaded-files");
    server.setUploadLock(true);
    server.setAutoindex(true);
    server.setLocations(locations);
    return server;
}

Server build_server_one(int port) {
    Server server = Server();
    std::vector<Location> locations = server.getLocations();

    std::map<int, std::string> error_page_413;
    error_page_413.insert(std::pair<int, std::string>(413, "./public/error-pages/413.html"));

    std::vector<std::string> allowed_methods_get_post;
    allowed_methods_get_post.push_back("GET");
    allowed_methods_get_post.push_back("POST");

    std::vector<std::string> allowed_methods_get;
    allowed_methods_get.push_back("GET");

    Location redirect_location = Location();
    redirect_location.setLimitExcept(allowed_methods_get);
    redirect_location.setRedirectLock(true);

    std::ostringstream redirection_path;
    redirection_path << "http://localhost:" << (port - 1);
    redirect_location.setRedirectLocation(redirection_path.str());

    Location hello_world_location = Location();
    hello_world_location.setPath("/hello");
    hello_world_location.setRoot("./public/hello-world");
    hello_world_location.setIndex("hello.html");
    hello_world_location.setAutoIndex(true);
    hello_world_location.setDirectoryRequestPage("./public/directory-page/alternative/index.html");
    hello_world_location.setErrorPages(error_page_413);
    hello_world_location.setLimitExcept(allowed_methods_get_post);
    hello_world_location.setMaxBodySize(10);

    locations.push_back(hello_world_location);
    locations.push_back(redirect_location);

    std::map<int, std::string> error_page_404;
    error_page_404.insert(std::pair<int, std::string>(404, "./public/error-pages/404.html"));

    server.setName("greeting.com");
    server.setIndex("index.html");
    server.setRoot("./");
    server.setDirectoryRequestPage("./public/directory-page/index.html");
    server.setAutoindex(true);
    server.setPort(port);
    server.setMaxBodySize(10000);
    server.setErrorPages(error_page_404);
    server.setLocations(locations);
    return server;
}

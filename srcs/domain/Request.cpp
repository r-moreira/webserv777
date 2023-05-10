//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/Request.h"

Request::Request(Event &event): _event(event) {}

Request::~Request() {}


void Request::read_request() {
    if (EventStateHelper::is_error_state(this->_event)) return;

    char buffer[READ_BUFFER_SIZE] = {};

    RequestInfo request;

    long bytes_read = read(this->_event.getClientFd(), buffer, READ_BUFFER_SIZE);

    if (bytes_read == -1) {
        std::cerr << RED << "Error while reading from client: " << strerror(errno) << RESET << std::endl;
        EventStateHelper::throw_error_state(this->_event, INTERNAL_SERVER_ERROR);
        return;
    } else if (bytes_read == 0) {
        std::cout << YELLOW << "Client disconnected" << RESET << std::endl;
        EventStateHelper::throw_error_state(this->_event, CLIENT_CLOSED_REQUEST);
        return;
    }

    this->_event.setRequestReadBytes(this->_event.getRequestReadBytes() + bytes_read);
    std::string read_buffer = this->_event.getRequestReadBuffer();
    this->_event.setRequestReadBuffer(read_buffer.append(buffer));

    std::cout << YELLOW << "Read " << this->_event.getRequestReadBytes() << " bytes from client" << RESET << std::endl;
    std::cout << GREEN << "HTTP Request:\n" << buffer << RESET << std::endl;

    if (buffer[READ_BUFFER_SIZE - 1] == 0) {
        this->_event.setEventSubStatus(ParsingRequest);
    }
}

void Request::parse_request() {
    if (EventStateHelper::is_error_state(this->_event)) return;

    std::cout << MAGENTA << "Request Data:\n " << this->_event.getRequestReadBuffer() << RESET << std::endl;

    const char *buffer = this->_event.getRequestReadBuffer().c_str();

    HttpRequestParser parser;

    HttpRequestParser::ParseResult result = parser.parse(_event._request, buffer, buffer + strlen(buffer));

    if (result == HttpRequestParser::ParsingCompleted) {
        std::cout << WHITE << "Parsed Request:\n" << _event.getRequest().inspect() << RESET << std::endl;
    } else {
        std::cerr << RED << "Parsing failed" << RESET << std::endl;
        EventStateHelper::throw_error_state(this->_event, BAD_REQUEST);
        return;
    }

    this->_event.setEventSubStatus(ChoosingServer);
}


void Request::choose_server(std::vector<Server> servers) {
    if (EventStateHelper::is_error_state(this->_event)) return;

    std::cout << BLUE << "Choosing Server:" << RESET << std::endl;

   /* for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
        if (it->getFd() == this->_event.getServerFd()) {
            this->_event.setServer(&*it);
            break;
        }
    }*/

    this->_event.setServer(servers[0]);

    std::cout << BLUE << "Choosed server = name: " <<
        this->_event.getServer().getName() << " port: " <<
        this->_event.getServer().getPort() << RESET << std::endl << std::endl;

    this->_event.setEventSubStatus(HandlingLocation);
}

void Request::handle_location() {
    if (EventStateHelper::is_error_state(this->_event)) return;

    std::cout << CYAN << "Handling Location:" << CYAN << std::endl;

    std::map<std::string, Location> locations = this->_event.getServer().getLocations();

    if (locations.empty()) {
        std::cerr << RED << "No locations found" << RESET << std::endl;
        EventStateHelper::throw_error_state(this->_event, INTERNAL_SERVER_ERROR);
        return;
    }

    bool found = false;
    /*for (std::map<std::string, Location>::iterator it = locations.begin(); it != locations.end(); it++) {
        if (this->_event.getRequest().uri.rfind(it->first, 0) == 0) {
            this->_event.setLocation(&it->second);
            found = true;
            break;
        }
    }*/

    this->_event.setLocation(locations.begin()->second);
    found = true;

    if (!found) {
        std::cerr << RED << "Location not found" << RESET << std::endl;
        EventStateHelper::throw_error_state(this->_event, NOT_FOUND);
        return;
    }

    std::string original = this->_event.getRequest().uri;
    std::string substring = this->_event.getLocation().getPath();
    std::size_t ind = original.find(substring); // Find the starting position of substring in the string
    if(ind !=std::string::npos){
        original.erase(ind,substring.length()); // erase function takes two parameter, the starting index in the string from where you want to erase characters and total no of characters you want to erase.
        std::cout<<original<<"\n";
    }else{
        std::cout<<"Substring does not exist in the string\n";
    }

    this->_event.setFilePath(this->_event.getLocation().getRoot() + original);


   /* std::cout << CYAN << "Replacing Path To Root" << CYAN << std::endl;
    std::string file_path = repace_path_to_root(
            this->_event.getRequest().uri,
            this->_event.getLocation()->getPath(),
            this->_event.getLocation()->getRoot());

    this->_event.setFilePath(file_path);*/

    //TODO: Traduzir o path do request para root

    this->_event.setEventSubStatus(ValidatingConstraints);
}

/*
 *
    std::string original = this->_event.getRequest().uri;
    std::string substring = this->_event.getLocation()->getPath();
    std::size_t ind = original.find(substring); // Find the starting position of substring in the string
    if(ind !=std::string::npos){
        original.erase(ind,substring.length()); // erase function takes two parameter, the starting index in the string from where you want to erase characters and total no of characters you want to erase.
        std::cout<<original<<"\n";
    }else{
        std::cout<<"Substring does not exist in the string\n";
    }

    this->_event.setFilePath("./public" + original);

 */

void Request::validate_constraints() {
    if (EventStateHelper::is_error_state(this->_event)) return;

    if (this->_event.getRequest().method != "GET" && this->_event.getRequest().method != "POST") {
        EventStateHelper::throw_error_state(this->_event, METHOD_NOT_ALLOWED);
        return;
    }

    if (this->_event.getServer().getMaxBodySize() != -1) {
        std::vector<RequestInfo::HeaderItem> headers = this->_event.getRequest().headers;

        for (std::vector<RequestInfo::HeaderItem>::iterator it = headers.begin(); it != headers.end(); it++) {
            if (it->name == "Content-Length") {
                long content_length = std::strtol(it->value.c_str(), NULL, 10);

                if (content_length > this->_event.getServer().getMaxBodySize()) {
                    EventStateHelper::throw_error_state(this->_event, PAYLOAD_TOO_LARGE);
                    return;
                }
            }
        }
    }

    this->_event.setEventStatus(Writing);
    this->_event.setEventSubStatus(OpeningFile);
}

std::string Request::repace_path_to_root(std::string subject, const std::string& search,
                                         const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}
//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/Request.h"

Request::Request(Event &event): _event(event), _read(event) {}

Request::~Request() {}


void Request::read_request() {
    _read.read_request();
}

void Request::parse_request() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << MAGENTA << "Request Data:\n " << this->_event.getRequestReadBuffer() << RESET << std::endl;

    const char *buffer = this->_event.getRequestReadBuffer().c_str();

    HttpRequestParser parser;

    HttpRequestParser::ParseResult result = parser.parse(_event._request, buffer, buffer + strlen(buffer));

    if (result == HttpRequestParser::ParsingCompleted) {
        std::cout << WHITE << "Parsed Request:\n" << _event.getRequest().inspect() << RESET << std::endl;
    } else {
        std::cerr << RED << "Parsing failed" << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, BAD_REQUEST);
        return;
    }

    this->_event.setEventSubStatus(ChoosingServer);
}


void Request::choose_server(std::vector<Server> servers) {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << BLUE << "Choosing Server:" << RESET << std::endl;

    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
        if (it->getFd() == this->_event.getServerFd()) {
            this->_event.setServer(*it);
            break;
        }
    }

    std::cout << BLUE << "Choosed server = name: " <<
        this->_event.getServer().getName() << " port: " <<
        this->_event.getServer().getPort() << RESET << std::endl << std::endl;

    this->_event.setEventSubStatus(ChoosingLocation);
}

void Request::choose_location() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << CYAN << "Handling Location:" << CYAN << std::endl;

    std::map<std::string, Location> locations = this->_event.getServer().getLocations();

    if (locations.empty()) {
        std::cerr << RED << "No locations found" << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, INTERNAL_SERVER_ERROR);
        return;
    }

    bool found = false;
    for (std::map<std::string, Location>::iterator it = locations.begin(); it != locations.end(); it++) {
        if (this->_event.getRequest().uri.rfind(it->first, 0) == 0) {
            this->_event.setLocation(it->second);
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << RED << "Location not found" << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, NOT_FOUND);
        return;
    }

    this->_event.setEventSubStatus(ValidatingConstraints);
}

void Request::validate_constraints() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << MAGENTA << "Validating Allowed Methods" << RESET << std::endl;
    std::vector<std::string> allowed_methods = this->_event.getLocation().getLimitExcept();
    if (std::find(allowed_methods.begin(), allowed_methods.end(), this->_event.getRequest().method) == allowed_methods.end()) {
        ErrorState::throw_error_state(this->_event, METHOD_NOT_ALLOWED);
        return;
    }

    if (this->_event.getServer().getMaxBodySize() != -1) {
        std::cout << MAGENTA << "Validating Content Length" << RESET <<std::endl;

        std::vector<RequestInfo::HeaderItem> headers = this->_event.getRequest().headers;

        for (std::vector<RequestInfo::HeaderItem>::iterator it = headers.begin(); it != headers.end(); it++) {
            if (it->name == "Content-Length") {
                long content_length = std::strtol(it->value.c_str(), NULL, 10);

                if (content_length > this->_event.getServer().getMaxBodySize()) {
                    ErrorState::throw_error_state(this->_event, PAYLOAD_TOO_LARGE);
                    return;
                }
            }
        }
    }

    this->_event.setEventStatus(Writing);
    this->_event.setEventSubStatus(DefiningResponseState);
}

//TODO: Definir o tipo da location e agir de acordo, nesse caso todas são do tipo regular, ou seja, requisitando aquivo, após trocar o path por root
void Request::define_response_state() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << CYAN << "Defining Response State" << RESET << std::endl;

    std::cout << MAGENTA << "Evento regular, fluxo: Path To Root" << RESET << std::endl;
    std::string file_path = path_to_root(
            this->_event.getRequest().uri,
            this->_event.getLocation().getPath(),
            this->_event.getLocation().getRoot());

    this->_event.setFilePath(file_path);

    //Mandar para o estado de acordo com o tipo de request/location
    this->_event.setEventSubStatus(SendingResponseFile);

    this->_event.setEventStatus(Writing);
}

std::string Request::path_to_root(std::string request_uri, const std::string& request_path,
                                  const std::string& location_root) {

    if (this->_event.getLocation().getPath() == "/") {
        return location_root + request_uri;
    }

    size_t pos = 0;
    while ((pos = request_uri.find(request_path, pos)) != std::string::npos) {
        request_uri.replace(pos, request_path.length(), location_root);
        pos += location_root.length();
    }
    return request_uri;
}

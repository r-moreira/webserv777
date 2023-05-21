//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/Request.h"

Request::Request(Event &event): _event(event), _read(event) {}

Request::~Request() {}


void Request::read_request() {
    _read.read_request();
    this->parse_request();
}

void Request::parse_request() {
    if (ErrorState::is_error_state(this->_event)) return;

    const char *buffer = this->_event.getRequestReadBuffer().c_str();
    RequestParser::ParseState parse_state;
    RequestParser parser;

    size_t bytes_read = 0;
    while (buffer && *buffer) {
        char c = *buffer;

        parse_state = parser.parse(_event._request, c);


        if (parse_state == RequestParser::ParsingCompleted) {
            std::cout << WHITE << "Parsed Request:\n" << _event.getRequest().inspect() << RESET << std::endl;
            this->_event.setEventSubStatus(Event::ChoosingServer);

            if (_event.getRequest().isIsFileUpload()) {
                bytes_read++;
                *buffer++;
                this->_event.setRequestReadBytes(REQUEST_READ_BUFFER_SIZE - bytes_read);
                std::string buffer_str(buffer, REQUEST_READ_BUFFER_SIZE - bytes_read);
                this->_event.setRemainingReadBuffer(buffer_str);
                std::cout << YELLOW << "Remaining Bytes: " << this->_event.getRequestReadBytes() << RESET << std::endl;
                std::cout << YELLOW << "Remaining Buffer: |" << this->_event.getRemainingReadBuffer() << "|" << RESET<< std::endl;

            }
            return;
        }

        if (parse_state == RequestParser::ParsingError || parse_state == RequestParser::ParsingIncompleted){
            std::cerr << RED << "Parsing failed:\n" << RESET << _event.getRequest().inspect() << std::endl;
            ErrorState::throw_error_state(this->_event, Event::BAD_REQUEST);
            return;
        }

        *buffer++;
        bytes_read++;
    }

    if (parse_state != RequestParser::ParsingCompleted) {
        std::cout << RED << "Parsing Error" << RESET << std::endl;
        std::cout << RED << "Parsed Request:\n" << _event.getRequest().inspect() << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::BAD_REQUEST);
        return;
    }
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

    std::cout << BLUE << "Choosed server =\n" << this->_event.getServer() << RESET << std::endl << std::endl;

    this->_event.setEventSubStatus(Event::ChoosingLocation);
}

void Request::choose_location() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << CYAN << "Handling Location:" << CYAN << std::endl;

    std::vector<Location> locations = this->_event.getServer().getLocations();

    if (locations.empty()) {
        locations.push_back(Location());
    }

    bool found = false;
    size_t location_path_size = 0;
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
        if (this->_event.getRequest().getUri().rfind(it->getPath(), 0) == 0) {

            if (it->getPath().size() > location_path_size) {
                this->_event.setLocation(*it);
                location_path_size = it->getPath().size();
                found = true;
            }
        }
    }

    if (!found) {
        std::cerr << RED << "Location not found" << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::NOT_FOUND);
        return;
    }

    std::cout << YELLOW << "Choosed location <" << std::endl << this->_event.getLocation() << RESET << std::endl;

    this->_event.setEventSubStatus(Event::ValidatingConstraints);
}

void Request::validate_constraints() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << MAGENTA << "Validating Allowed Methods" << RESET << std::endl;
    std::vector<std::string> allowed_methods = this->_event.getLocation().getLimitExcept().size() < 3
                                               ? this->_event.getLocation().getLimitExcept()
                                               : this->_event.getServer().getLimitExcept();

    if (std::find(allowed_methods.begin(), allowed_methods.end(), this->_event.getRequest().getMethod()) == allowed_methods.end()) {
        ErrorState::throw_error_state(this->_event, Event::METHOD_NOT_ALLOWED);
        return;
    }

    long max_body_size = this->_event.getLocation().getMaxBodySize() != -1
            ? this->_event.getLocation().getMaxBodySize()
            : this->_event.getServer().getMaxBodySize();

    if (max_body_size != -1) {
        std::cout << MAGENTA << "Validating Content Length" << RESET <<std::endl;

        std::vector<RequestData::HeaderItem> headers = this->_event.getRequest().getHeaders();

        for (std::vector<RequestData::HeaderItem>::iterator it = headers.begin(); it != headers.end(); it++) {
            if (it->name == "Content-Length") {
                long content_length = std::strtol(it->value.c_str(), NULL, 10);

                if (content_length > max_body_size) {
                    ErrorState::throw_error_state(this->_event, Event::PAYLOAD_TOO_LARGE);
                    return;
                }
            }
        }
    }

    this->_event.setEventStatus(Event::Writing);
    this->_event.setEventSubStatus(Event::DefiningResponseState);
}

//TODO:: Adicionar o estados restantes
// if request method = POST -> upload file
void Request::define_response_state() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << CYAN << "Defining Response State" << RESET << std::endl;

    //Se o path da requisição for igual ao path da location e terminar sem a "/" é necessário fazer um redirect para adicionando a "/"
    //  se não o navegador vai requisitar o diretório incorreto.
    std::string request_uri = this->_event.getRequest().getUri();
    if (request_uri.length() > 1 && request_uri[request_uri.length() - 1] != '/' && request_uri == this->_event.getLocation().getPath()) {
        std::cout << MAGENTA << "Forcing redirect to location with /" << RESET << std::endl;
        std::string redirect_uri = request_uri + "/";

        this->_event.setForcedRedirect(true);
        this->_event.setForcedRedirectLocation(redirect_uri);
        this->_event.setEventSubStatus(Event::SendingRedirectionResponse);
        this->_event.setEventStatus(Event::Writing);
        return;
    }

    if (this->_event.getLocation().isRedirectLock()) {
        std::cout << MAGENTA << "Redirection Event" << RESET << std::endl;

        this->_event.setEventSubStatus(Event::SendingRedirectionResponse);
        this->_event.setEventStatus(Event::Writing);
        return;
    }

    if (this->_event.getRequest().getMethod() == "POST") {
        std::cout << MAGENTA << "Upload Event" << RESET << std::endl;
        this->_event.setEventSubStatus(Event::SendingUploadResponse);
        this->_event.setEventStatus(Event::Writing);
        return;
    }



    std::string file_path = path_to_root();
    bool is_auto_index = this->_event.getLocation().isAutoIndex() || this->_event.getServer().isAutoindex();

    if (is_directory(file_path)) {
        std::cerr << RED << "Redirecionado para página de erro de diretório" << RESET << std::endl;
        this->_event.setEventSubStatus(Event::SendingDirectoryResponse);
        this->_event.setHttpStatus(Event::FORBIDDEN);
        this->_event.setEventStatus(Event::Writing);
        return;
    }

 /*if (is_dir_and_not_index && !is_auto_index) {
     std::cerr << RED << "Redirecionado para página de erro de diretório" << RESET << std::endl;
     this->_event.setEventSubStatus(Event::SendingDirectoryResponse);
     this->_event.setHttpStatus(Event::FORBIDDEN);
     this->_event.setEventStatus(Event::Writing);
     return;
 } else if (is_dir_and_not_index && is_auto_index) {
     std::cout << MAGENTA << "Auto Index Event" << RESET << std::endl;
     this->_event.setEventSubStatus(Event::SendingAutoIndexResponse);
     this->_event.setEventStatus(Event::Writing);
     return;
 }*/

    this->_event.setFilePath(file_path);

    if (this->_event.getRequest().getMethod() == "DELETE") {
        std::cout << MAGENTA << "Delete Event" << RESET << std::endl;
        this->_event.setEventSubStatus(Event::SendingDeleteResponse);
        this->_event.setEventStatus(Event::Writing);
        return;
    }

    this->_event.setEventSubStatus(Event::SendingResponseFile);
    this->_event.setEventStatus(Event::Writing);

}

bool Request::is_directory(const std::string& path) {
    struct stat s;

    if (stat(path.c_str(), &s) == 0) {
        if (s.st_mode & S_IFDIR) {
            std::cout << YELLOW << "Path is a directory: " << path <<RESET << std::endl;
            return true;
        }
    }
    return false;
}

std::string Request::path_to_root() {
    std::cout << YELLOW << "Path to root" << RESET << std::endl;

    std::string request_uri = this->_event.getRequest().getUri();
    std::string location_path = this->_event.getLocation().getPath();

    std::string location_root = !this->_event.getLocation().getRoot().empty()
            ? this->_event.getLocation().getRoot()
            : this->_event.getServer().getRoot();

    std::string index = !this->_event.getLocation().getIndex().empty()
            ? this->_event.getLocation().getIndex()
            : this->_event.getServer().getIndex();

    std::cout << BLUE << "request_uri: " << request_uri << RESET << std::endl;
    std::cout << BLUE << "location_path: " << location_path << RESET << std::endl;
    std::cout << BLUE << "location_root: " << location_root << RESET << std::endl;
    std::cout << BLUE << "index: " << index << RESET << std::endl;

    std::string request_without_slash = request_uri.length() > 1 && request_uri[request_uri.length() - 1] == '/' ?
                                    request_uri.substr(0, request_uri.length() - 1) : request_uri;

    if (request_without_slash == location_path && is_index_exists_in_directory(location_path, location_root, index)) {
        request_uri = request_uri + index;
    }

    if (this->_event.getLocation().getPath() == "/") {
        return location_root + request_uri;
    }

    size_t pos = 0;
    pos = request_uri.find(location_path, pos);
    request_uri.replace(pos, location_path.length(), location_root);

    return request_uri;
}

bool Request::is_index_exists_in_directory(const std::string& path, const std::string& root, const std::string& index) {
    std::string rooted_location_path = path;
    rooted_location_path.replace(0, path.length(), root);

    std::string index_html = rooted_location_path + "/" + index;

    struct stat s;
    if (stat(index_html.c_str(), &s) == 0) {
        if (s.st_mode & S_IFREG) {
            std::cout << YELLOW << "Index exists in directory: " << index_html << RESET << std::endl;
            return true;
        }
    }
    return false;
}
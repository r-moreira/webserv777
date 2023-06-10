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
    RequestParser::ParseState::State parse_state;
    RequestParser parser;

    size_t bytes_read = 0;
    while (buffer && *buffer) {
        char c = *buffer;

        parse_state = parser.parse(_event._request, c);

        if (parse_state == RequestParser::ParseState::ParsingCompleted) {
            Logger::debug("Parsed Request: \n" + _event.getRequest().inspect());
            this->_event.setEventSubStatus(Event::SubStatus::ChoosingServer);

            bytes_read++;
            (void)*buffer++;

            size_t request_read_remaining = this->_event.getRequestReadBytes() - bytes_read;
            size_t body_remaining_bytes = this->_event.getRequest().getBodyRemainingBytes() < request_read_remaining ?
                                          this->_event.getRequest().getBodyRemainingBytes() : request_read_remaining;
            std::string buffer_str(buffer, body_remaining_bytes);

            this->_event.setRequestReadBytes(body_remaining_bytes);
            this->_event.setRemainingReadBuffer(buffer_str);

            Logger::trace("Remaining Bytes: " + ITOSTR(this->_event.getRequestReadBytes()));
            Logger::trace("Remaining Buffer: |" + this->_event.getRemainingReadBuffer() + "|");
            return;
        }

        if (parse_state == RequestParser::ParseState::ParsingError || parse_state == RequestParser::ParseState::ParsingIncompleted){
            Logger::warning("Parsing failed:");
            Logger::trace("Request:\n" + _event.getRequest().inspect());
            ErrorState::throw_error_state(this->_event, Event::HttpStatus::BAD_REQUEST);
            return;
        }

        (void)*buffer++;
        bytes_read++;
    }

    if (parse_state != RequestParser::ParseState::ParsingCompleted) {
        Logger::warning("Parsing failed:");
        Logger::trace("Request:\n" + _event.getRequest().inspect());
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::BAD_REQUEST);
        return;
    }
}

void Request::choose_server(std::vector<Server> servers) {
    if (ErrorState::is_error_state(this->_event)) return;

    Logger::debug("Choosing Server:");

    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
        if (it->getFd() == this->_event.getServerFd()) {
            this->_event.setServer(*it);
            break;
        }
    }

    std::stringstream log;
    log << BLUE << "Choosed server =\n"<< this->_event.getServer() << RESET << std::endl << std::endl;
    Logger::trace(log.str());

    this->_event.setEventSubStatus(Event::SubStatus::ChoosingLocation);
}

void Request::choose_location() {
    if (ErrorState::is_error_state(this->_event)) return;

    Logger::debug("Choosing Location:");

    std::vector<Location> locations = this->_event.getServer().getLocations();

    bool found = false;

    if (!locations.empty()) {
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
    }

    if (!found || locations.empty()) {
        Logger::warning("Location not found, using defaults");
        this->_event.setLocation(Location());

        return;
    }

    std::stringstream log;
    log << YELLOW << "Choosed location >\n" << this->_event.getLocation() << RESET << std::endl;
    Logger::trace(log.str());

    this->_event.setEventSubStatus(Event::SubStatus::ValidatingConstraints);
}

void Request::validate_constraints() {
    if (ErrorState::is_error_state(this->_event)) return;

    Logger::debug("Validating Allowed Methods");

    std::vector<std::string> allowed_methods = this->_event.getLocation().getLimitExcept().size() < 3
                                               ? this->_event.getLocation().getLimitExcept()
                                               : this->_event.getServer().getLimitExcept();

    if (std::find(allowed_methods.begin(), allowed_methods.end(), this->_event.getRequest().getMethod()) == allowed_methods.end()) {
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::METHOD_NOT_ALLOWED);
        return;
    }

    long max_body_size = this->_event.getLocation().getMaxBodySize() != -1
            ? this->_event.getLocation().getMaxBodySize()
            : this->_event.getServer().getMaxBodySize();

    if (max_body_size != -1) {

        Logger::debug("Validating Content Length");

        std::vector<RequestData::HeaderItem> headers = this->_event.getRequest().getHeaders();

        for (std::vector<RequestData::HeaderItem>::iterator it = headers.begin(); it != headers.end(); it++) {
            if (it->name == "Content-Length") {
                long content_length = std::strtol(it->value.c_str(), NULL, 10);

                if (content_length > max_body_size) {
                    ErrorState::throw_error_state(this->_event, Event::HttpStatus::PAYLOAD_TOO_LARGE);
                    return;
                }
            }
        }
    }

    this->_event.setEventStatus(Event::Status::Writing);
    this->_event.setEventSubStatus(Event::SubStatus::DefiningResponseState);
}

void Request::define_response_state() {
    if (ErrorState::is_error_state(this->_event)) return;

    Logger::debug("Defining Response State");

    std::string request_uri = this->_event.getRequest().getUri();

    if (request_uri.length() > 1
        && request_uri[request_uri.length() - 1] != '/'
        && request_uri == this->_event.getLocation().getPath()
        && this->_event.getRequest().getMethod() == "GET") {

        Logger::warning("Forcing redirect to location with /");
        std::string redirect_uri = request_uri + "/";

        this->_event.setForcedRedirect(true);
        this->_event.setForcedRedirectLocation(redirect_uri);
        this->_event.setEventSubStatus(Event::SubStatus::SendingRedirectionResponse);
        this->_event.setEventStatus(Event::Status::Writing);
        return;
    }

    if (this->_event.getLocation().isRedirectLock()) {
        Logger::trace("Redirecion Event");

        this->_event.setEventSubStatus(Event::SubStatus::SendingRedirectionResponse);
        this->_event.setEventStatus(Event::Status::Writing);
        return;
    }

    bool file_upload_lock = this->_event.getLocation().isUploadLock()
            || (this->_event.getServer().isUploadLock() && this->_event.getLocation().getPath() == "/");

    if (this->_event.getRequest().getMethod() == "POST" && file_upload_lock && !this->_event.getLocation().isCgiLock()) {
        Logger::trace("Upload Event");
        this->_event.setEventSubStatus(Event::SubStatus::SendingUploadResponse);
        this->_event.setEventStatus(Event::Status::Writing);
        return;
    } else if (this->_event.getRequest().getMethod() == "POST" && this->_event.getLocation().isCgiLock()) {
        Logger::trace("POST CGI Event");
        this->_event.setEventSubStatus(Event::SubStatus::SendingCGIResponse);
        this->_event.setEventStatus(Event::Status::Writing);
        return;
    } else if (this->_event.getRequest().getMethod() == "POST"){
        Logger::warning("Forbidden Event: Post Request allowed only for File Upload or CGI");
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::FORBIDDEN);
        return;
    }

    if (this->_event.getRequest().getMethod() == "GET" && this->_event.getLocation().isCgiLock()) {
        Logger::trace("GET CGI Event");
        this->_event.setEventSubStatus(Event::SubStatus::SendingCGIResponse);
        this->_event.setEventStatus(Event::Status::Writing);
        return;
    }

    std::string file_path = path_to_root();
    this->_event.setFilePath(file_path);

     bool is_auto_index = this->_event.getLocation().getAutoIndexOption() != AutoIndexOption::NONE
        ? this->_event.getLocation().getAutoIndexOption() == AutoIndexOption::ON
        : this->_event.getServer().getAutoIndexOption() == AutoIndexOption::ON;

    bool is_dir = is_directory(file_path);

    if (is_dir && !is_auto_index) {
         Logger::trace("Redirecting to directory error page");
         this->_event.setEventSubStatus(Event::SubStatus::SendingDirectoryResponse);
         this->_event.setHttpStatus(Event::HttpStatus::FORBIDDEN);
         this->_event.setEventStatus(Event::Status::Writing);
         return;
    }

    if (is_dir) {
         Logger::trace("Auto Index Event");
         this->_event.setEventSubStatus(Event::SubStatus::SendingAutoIndexResponse);
         this->_event.setEventStatus(Event::Status::Writing);
         return;
    }

    if (this->_event.getRequest().getMethod() == "DELETE") {
        Logger::trace("Delete Event");
        this->_event.setEventSubStatus(Event::SubStatus::SendingDeleteResponse);
        this->_event.setEventStatus(Event::Status::Writing);
        return;
    }

    if (this->_event.getRequest().getMethod() == "GET") {
        Logger::trace("Request File Event");
        this->_event.setEventSubStatus(Event::SubStatus::SendingResponseFile);
        this->_event.setEventStatus(Event::Status::Writing);
        return;
    }

    Logger::warning("Not Implemented Request");
    ErrorState::throw_error_state(this->_event, Event::HttpStatus::NOT_IMPLEMENTED);
}

bool Request::is_directory(const std::string& path) {
    struct stat s;

    if (stat(path.c_str(), &s) == 0) {
        if (s.st_mode & S_IFDIR) {
            Logger::warning("Path is a directory: " + path);
            return true;
        }
    }
    return false;
}

std::string Request::path_to_root() {
    Logger::trace("Path to root");

    std::string request_uri = this->_event.getRequest().getUri();
    std::string location_path = this->_event.getLocation().getPath();

    std::string location_root = !this->_event.getLocation().getRoot().empty()
            ? this->_event.getLocation().getRoot()
            : this->_event.getServer().getRoot();

    std::string index = !this->_event.getLocation().getIndex().empty()
            ? this->_event.getLocation().getIndex()
            : this->_event.getServer().getIndex();

    Logger::trace("request_uri: " + request_uri);
    Logger::trace("location_path: " + location_path);
    Logger::trace("location_root: " + location_root);
    Logger::trace("index: " + index);

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
            Logger::trace("Index exists in directory: " + index_html);
            return true;
        }
    }
    return false;
}
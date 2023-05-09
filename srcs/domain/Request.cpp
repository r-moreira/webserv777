//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/Request.h"

Request::Request(Event &event): _event(event) {}

Request::~Request() {}

void Request::parse_request() {
    if (this->_event.getEventStatus() == Ended) return;

    std::cout << MAGENTA << "Request Data:\n " << this->_event.getRequestReadBuffer() << RESET << std::endl;

    const char *buffer = this->_event.getRequestReadBuffer().c_str();

    HttpRequestParser parser;

    HttpRequestParser::ParseResult result = parser.parse(_event._request, buffer, buffer + strlen(buffer));

    if (result == HttpRequestParser::ParsingCompleted) {
        std::cout << WHITE << "Parsed Request:\n" << _event.getRequest().inspect() << RESET << std::endl;
    } else {
        std::cerr << RED << "Parsing failed" << RESET << std::endl;
        this->_event.setEventStatus(Ended);
    }

    this->_event.setEventSubStatus(ChoosingServer);
}

void Request::read_request() {
    if (this->_event.getEventStatus() == Ended) return;

    char buffer[READ_BUFFER_SIZE] = {};
    RequestInfo request;

    long bytes_read = read(this->_event.getClientFd(), buffer, READ_BUFFER_SIZE);

    if (bytes_read == -1) {
        std::cerr << RED << "Error while reading from client: " << strerror(errno) << RESET << std::endl;
        this->_event.setEventStatus(Ended);
    } else if (bytes_read == 0) {
        std::cout << YELLOW << "Client disconnected" << RESET << std::endl;
        this->_event.setEventStatus(Ended);
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

void Request::choose_server(std::vector<Server> servers) {
    std::cout << BLUE << "\nChoosing Server:" << RESET << std::endl;

    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
        if (it->getFd() == this->_event.getServerFd()) {
            this->_event.setServer(&*it);
            break;
        }
    }

    std::cout << BLUE << "Choosed server = name: " <<
        this->_event.getServer()->getName() << " port: " <<
        this->_event.getServer()->getPort() << RESET << std::endl << std::endl;

    this->_event.setEventSubStatus(ValidatingConstraints);
}

// Por enquanto só vai ter um sub estado de leitura de arquivo, mas no futuro terá outros dependendo da funcionalidade
void Request::validate_constraints() {

    //this->_event.setHttpStatus(INTERNAL_SERVER_ERROR);
    //this->_event.setEventStatus(Writing);
    //this->_event.setEventSubStatus(WritingErrorHeaders);

    this->_event.setEventStatus(Writing);
    this->_event.setEventSubStatus(OpeningFile);


}

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
        //Return error page, end connection
        this->_event.setEventStatus(Ended);
    }
    this->_event.setEventStatus(Writing);

    // Por enquanto só vai ter um sub estado de leitura de arquivo, mas no futuro terá outros dependendo da funcionalidade
    // TODO make a cgi request parcer here
    this->_event.setEventSubStatus(OpeningFile);
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
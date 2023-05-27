//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/EventHandler.h"

EventHandler::EventHandler(Event &event, std::vector<Server> &servers) :
    _servers(servers),
    _event(event),
    _request(event),
    _response(event),
    _file(event) {}

EventHandler::~EventHandler() {}

//TODO Server Features:
// CGI retornar erro no loop infinito
// Fazer o parser de configuração instânciar os servidores e locations
// Entender o que significa e implementar :
//      Setup multiple servers with different hostnames (use something like: curl --resolve
//      example.com:80:127.0.0.1 http://example.com/ (http://example.com/)).
// Testar com todas configurações possíveis
// Remover logs desnecessários, criar um padrão de log para o restante

void EventHandler::process_event() {

    if (this->_event.getEventStatus() == Event::Status::Reading) {
        switch (this->_event.getEventSubStatus()) {
            case Event::SubStatus::ReadingRequest: _request.read_request(); /* falls through */
            case Event::SubStatus::ChoosingServer: _request.choose_server(this->_servers); /* falls through */
            case Event::SubStatus::ChoosingLocation: _request.choose_location(); /* falls through */
            case Event::SubStatus::ValidatingConstraints: _request.validate_constraints(); /* falls through */
            case Event::SubStatus::DefiningResponseState: _request.define_response_state(); /* falls through */
                break;
            default:
                std::cerr << RED << "Invalid Reading Event Status" << RESET << std::endl;
                break;
        }
    }

    if (this->_event.getEventStatus() == Event::Status::Writing) {
        switch (this->_event.getEventSubStatus()) {
            case Event::SubStatus::SendingResponseFile: _response.send_response_file();
                break;
            case Event::SubStatus::SendingUploadResponse: _response.send_upload_response();
                break;
            case Event::SubStatus::SendingDeleteResponse: _response.send_delete_response();
                break;
            case Event::SubStatus::SendingRedirectionResponse: _response.send_redirection_response();
                break;
            case Event::SubStatus::SendingAutoIndexResponse: _response.send_auto_index_response();
                break;
            case Event::SubStatus::SendingCGIResponse: _response.send_cgi_response();
                break;
            case Event::SubStatus::SendingDirectoryResponse: _response.send_is_directory_response();
                break;
            case Event::SubStatus::SendingErrorResponse: _response.send_error_response();
                break;
            default:
                std::cerr << RED << "Invalid Writing Event Sub Status:" << RESET << std::endl;
                break;
        }
    }

    this->print_event_status();
}

void EventHandler::print_event_status() {

    std::string status;
    std::string sub_status;

    switch (this->_event.getEventStatus()) {
        case Event::Status::Reading: status = "Reading";
            break;
        case Event::Status::Writing: status = "Writing";
            break;
        case Event::Status::Ended: status = "Ended";
            break;
        default: status = "Invalid";
            break;
    }

    switch (this->_event.getEventSubStatus()) {
        case Event::SubStatus::ReadingRequest: sub_status = "ReadingRequest";
            break;
        case Event::SubStatus::ParsingRequest: sub_status = "ParsingRequest";
            break;
        case Event::SubStatus::ChoosingServer: sub_status = "ChoosingServer";
            break;
        case Event::SubStatus::ChoosingLocation: sub_status = "ChoosingLocation";
            break;
        case Event::SubStatus::DefiningResponseState: sub_status = "DefiningResponseState";
            break;
        case Event::SubStatus::SendingResponseFile: sub_status = "SendingResponseFile";
            break;
        case Event::SubStatus::SendingUploadResponse: sub_status = "SendingUploadResponse";
            break;
        case Event::SubStatus::SendingDeleteResponse: sub_status = "SendingDeleteResponse";
            break;
        case Event::SubStatus::SendingRedirectionResponse: sub_status = "SendingRedirectionResponse";
            break;
        case Event::SubStatus::SendingAutoIndexResponse: sub_status = "SendingAutoIndexResponse";
            break;
        case Event::SubStatus::SendingCGIResponse: sub_status = "SendingCGIResponse";
            break;
        case Event::SubStatus::SendingDirectoryResponse: sub_status = "SendingDirectoryResponse";
            break;
        case Event::SubStatus::SendingErrorResponse: sub_status = "SendingErrorResponse";
            break;
        default: sub_status = "Invalid";
            break;
    }

    std::cout << std::endl << "Event Status: " << status << std::endl;
    std::cout <<  "Event Sub Status: " << sub_status << std::endl;
}

const Event &EventHandler::getEvent() const {
    return _event;
}

void EventHandler::setEvent(const Event &event) {
    this->_event = event;
}

//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/EventHandler.h"

EventHandler::EventHandler(Event &event, std::vector<Server> &servers) :_servers(servers), _event(event), _request(event), _response(event), _file(event) {}

EventHandler::~EventHandler() {}

//TODO Server Features:
// Implementar suporte a location redirect, cgi, upload e auto_index (Via server e location)
// Fazer o parser de configuração instânciar os servidores e locations
// Refatorar o parse da request http
// Apagar o arquivo quando o método for DELETE
// Entender o que significa e implementar :
//      Setup multiple servers with different hostnames (use something like: curl --resolve
//      example.com:80:127.0.0.1 http://example.com/ (http://example.com/)).
// Testar com todas configurações possíveis
//      Checar os uso dos getters e setters do servidores e locations é uma forma de saber se tudo foi testado

//TODO Atuais:
// Feature: Arquivos com configurações validas no escopo do server e location
//      Suportar error_page (Via location) - Apenas suportado no server atualmente
//      Suportar directory page (Via location) - Apenas suportado no server atualmente
void EventHandler::process_event() {
    if (this->_event.getEventStatus() == Reading) {

        switch (this->_event.getEventSubStatus()) {
            case ReadingRequest: _request.read_request();
            case ParsingRequest: _request.parse_request();
            case ChoosingServer: _request.choose_server(this->_servers);
            case ChoosingLocation: _request.choose_location();
            case ValidatingConstraints: _request.validate_constraints();
            case DefiningResponseState: _request.define_response_state();
                break;
            default:
                std::cerr << RED << "Invalid Reading Event Status" << RESET << std::endl;
                break;
        }
    }

    if (this->_event.getEventStatus() == Writing) {
        switch (this->_event.getEventSubStatus()) {
            case SendingResponseFile: _response.send_response_file();
                break;
            //case UploadingFile: _response.upload_file();
                break;
            //case DeletingFile: _response.delete_file();
                break;
            case SendingRedirectionResponse: _response.send_redirection();
                break;
            //case SendingCgiResponse: _response.send_cgi();
                //break;
            // case SendingAutoIndexResponse: _response.send_auto_index();
                //break;
            case SendingDirectoryResponse: _response.send_is_directory_response();
                break;
            case SendingErrorResponse: _response.send_error_response();
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
        case Reading: status = "Reading";
            break;
        case Writing: status = "Writing";
            break;
        case Ended: status = "Ended";
            break;
        default: status = "Invalid";
            break;
    }

    switch (this->_event.getEventSubStatus()) {
        case ReadingRequest: sub_status = "ReadingRequest";
            break;
        case ParsingRequest: sub_status = "ParsingRequest";
            break;
        case ChoosingServer: sub_status = "ChoosingServer";
            break;
        case ChoosingLocation: sub_status = "ChoosingLocation";
            break;
        case DefiningResponseState: sub_status = "DefiningResponseState";
            break;
        case SendingRedirectionResponse: sub_status = "SendingRedirectionResponse";
            break;
        case SendingDirectoryResponse: sub_status = "SendingDirectoryResponse";
            break;
        case SendingResponseFile: sub_status = "SendingResponseFile";
            break;
        case SendingErrorResponse: sub_status = "SendingErrorResponse";
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

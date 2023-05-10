//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/EventHandler.h"

EventHandler::EventHandler(Event &event, std::vector<Server> &servers) :_servers(servers), _event(event), _request(event), _response(event), _file(event) {}

EventHandler::~EventHandler() {}

//TODO Features
// Implementar suporte a location redirect, cgi, upload e auto_index
// Implementar suporte a pagina de diretório customizada, em caso de erro de request para diretório
// Implementar suporte a pagina de erro customizada, em caso de erro geral
// Implementar suporte a upload de arquivos

//TODO Atuais:
// Fazer gerênciamento do estado do evento com base no location, retornar status code e pagina de exemplo
// Fazer leitura de arquivos de paginas de erro/diretório com base na configuração para poder responde-la ao client
// Fazer método para upload e arquivo no diretório da config
void EventHandler::process_event() {
    if (this->_event.getEventStatus() == Reading) {

        switch (this->_event.getEventSubStatus()) {
            case ReadingRequest: _request.read_request();
            case ParsingRequest: _request.parse_request();
            case ChoosingServer: _request.choose_server(this->_servers);
            case ChoosingLocation: _request.choose_location();
            case ValidatingConstraints: _request.validate_constraints();
            case DefiningResponseState: _request.define_response_state();
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
            //case SendingRedirectionResponse: _response.handle_redirection(); --> responder 302 para o navegador chamar outro site
                //break;
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

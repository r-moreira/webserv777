//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/EventHandler.h"

EventHandler::EventHandler(Event &event, std::vector<Server> &servers) :_servers(servers), _event(event), _request(event), _response(event), _file(event) {}

EventHandler::~EventHandler() {}

//TODO Server Features:
// Implementar cgi (Via location
// Implementar auto_index (Via server e location)
// Fazer o parser de configuração instânciar os servidores e locations
// Refatorar o parse da request http
// Entender o que significa e implementar :
//      Setup multiple servers with different hostnames (use something like: curl --resolve
//      example.com:80:127.0.0.1 http://example.com/ (http://example.com/)).
// Aceitar POST apenas para upload de arquivo e outras coisas necessárias se precisar (Como CGI)
// Trocar o throw error state por uma exception do C++ (https://www.cplusplus.com/doc/tutorial/exceptions/)
// Testar com todas configurações possíveis
//      Checar os uso dos getters e setters do servidores e locations é uma forma de saber se tudo foi testado

//TODO Atuais:
// Upload de arquivo
//      Como saber se o HTTP request é um arquivo:
//          Refs:https://stackoverflow.com/questions/14872460/how-to-detect-a-file-upload-examining-the-http-message:
//          Refs: https://stackoverflow.com/questions/36184410/how-does-webserver-handle-http-post
//          Refs: https://stackoverflow.com/questions/11254037/how-to-know-when-the-http-headers-part-is-ended
//      TODOs:
//          Fazer o parse do body na classe RequestParser. Verificar se o Content-Type é multipart/form-data para saber se é um arquivo
//          A ideia é ter o content-length já calculado exatamente com o tamanho do arquivo. (Removendo os boundaries de início, fim e headers)
//          Também será necessário ter o nome do arquivo e content-type
//          É possível que seja recebido um _event._remaining_read_buffer, que precisará ser escrito no arquivo antes de solicitar novamente outra leitura no socket
//          A ideia é que o parser já deixe ramaining_read_buffer apontando exatamente para o início do arquivo, sem os boundaries e headers
//      Avaliar:
//          Existe a possibildade de enviar arquivo com Content-Type com direto do arquivo, sem ser multipart/form-data, nesse caso teria que pegar o nome é a extensão via path da url, talvez não seja obrigatório fazer isso

// Apagar o arquivo do path quando o método for DELETE

// Test: Criar uma rota GET DELETE com location apontando para o diretório de upload no servidor de upload/cgi


void EventHandler::process_event() {
    if (this->_event.getEventStatus() == Event::Reading) {

        switch (this->_event.getEventSubStatus()) {
            case Event::ReadingRequest: _request.read_request();
            case Event::ChoosingServer: _request.choose_server(this->_servers);
            case Event::ChoosingLocation: _request.choose_location();
            case Event::ValidatingConstraints: _request.validate_constraints();
            case Event::DefiningResponseState: _request.define_response_state();
                break;
            default:
                std::cerr << RED << "Invalid Reading Event Status" << RESET << std::endl;
                break;
        }
    }

    if (this->_event.getEventStatus() == Event::Writing) {
        switch (this->_event.getEventSubStatus()) {
            case Event::SendingResponseFile: _response.send_response_file();
                break;
            case Event::SendingUploadResponse: _response.send_upload_response();
                break;
            //case SendingDeletingFileResponse: _response.send_delete_file_response()();
                break;
            case Event::SendingRedirectionResponse: _response.send_redirection();
                break;
            //case SendingCgiResponse: _response.send_cgi();
                //break;
            // case SendingAutoIndexResponse: _response.send_auto_index();
                //break;
            case Event::SendingDirectoryResponse: _response.send_is_directory_response();
                break;
            case Event::SendingErrorResponse: _response.send_error_response();
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
        case Event::Reading: status = "Reading";
            break;
        case Event::Writing: status = "Writing";
            break;
        case Event::Ended: status = "Ended";
            break;
        default: status = "Invalid";
            break;
    }

    switch (this->_event.getEventSubStatus()) {
        case Event::ReadingRequest: sub_status = "ReadingRequest";
            break;
        case Event::ParsingRequest: sub_status = "ParsingRequest";
            break;
        case Event::ChoosingServer: sub_status = "ChoosingServer";
            break;
        case Event::ChoosingLocation: sub_status = "ChoosingLocation";
            break;
        case Event::DefiningResponseState: sub_status = "DefiningResponseState";
            break;
        case Event::SendingRedirectionResponse: sub_status = "SendingRedirectionResponse";
            break;
        case Event::SendingDirectoryResponse: sub_status = "SendingDirectoryResponse";
            break;
        case Event::SendingResponseFile: sub_status = "SendingResponseFile";
            break;
        case Event::SendingErrorResponse: sub_status = "SendingErrorResponse";
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

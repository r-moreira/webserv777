//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/EventHandler.h"

EventHandler::EventHandler(Event &event, std::vector<Server> &servers) :_servers(servers), _event(event), _request(event), _response(event), _file(event) {}

EventHandler::~EventHandler() {}

void EventHandler::process_event() {
    if (this->_event.getEventStatus() == Reading) {

        switch (this->_event.getEventSubStatus()) {
            case ReadingRequest: _request.read_request();
            case ParsingRequest: _request.parse_request();
            case ChoosingServer: _request.choose_server(this->_servers);
            case ValidatingConstraints: _request.validate_constraints(); //TODO
            //case HandlingLocation: _request.handle_location();   --> Traduzir o path, lidar com redirects
                break;
            default:
                std::cerr << RED << "Invalid Reading Event Status" << RESET << std::endl;
                break;
        }
    }

    if (this->_event.getEventStatus() == Writing) {
        switch (this->_event.getEventSubStatus()) {
            case OpeningFile: _file.open_file();
            case WritingResponseHeaders: _response.write_response_headers();
            case WritingResponseFile: _response.write_response_file();
                break;
            //case HandlingRedirection: _response.handle_redirection(); --> responder 302 para o navegador chamar outro site
                //break;
            //case WritingCGI: _response.write_cgi();
                //break;
            // case WritingAutoIndex: _response.write_auto_index();
                //break;
            //case WritingDirectoryPage: _response.write_directory_page();

            case WritingErrorHeaders: _response.write_error_headers();
            case WritingErrorPage: _response.write_error_page();
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
        case OpeningFile: sub_status = "OpeningFile";
            break;
        case WritingResponseHeaders: sub_status = "WritingResponseHeaders";
            break;
        case WritingResponseFile: sub_status = "WritingResponseFile";
            break;
        case WritingErrorHeaders: sub_status = "WritingErrorHeaders";
            break;
        case WritingErrorPage: sub_status = "WritingErrorPage";
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

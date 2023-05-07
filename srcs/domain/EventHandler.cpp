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
            case UploadingFile: _response.upload_file();
                break;
            default:
                std::cerr << RED << "Invalid Writing Event Sub Status" << RESET << std::endl;
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
        case UploadingFile: sub_status = "UploadingFile";
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

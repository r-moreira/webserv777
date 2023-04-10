//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/EventHandler.h"

EventHandler::EventHandler(Event &event) : _event(event), _request(event), _response(event) {}

EventHandler::~EventHandler() {}


void EventHandler::open_file() {
    if (this->_event.getEventStatus() == Ended) return;

    if (this->_event.getFile() == NULL) {
        this->_event.setFilePath("./public" + this->_event.getRequest().getUri());

        FILE *fptr;
        fptr = fopen(this->_event.getFilePath().c_str(), "rb");
        if (fptr == NULL) {
            std::cerr << RED << "Error while opening file: " << this->_event.getFilePath() << " " << strerror(errno) << RESET << std::endl;
            //return error page, end connection
            this->_event.setEventStatus(Ended);
        }
        this->_event.setFile(fptr);
        //Por algum motivo readbytes precisa ser inicializado neste momento
        this->_event.setFileReadBytes(0);
    }

    struct stat file_stat;
    int fd = fileno(this->_event.getFile());
    if (fd < 1) {
        std::cerr << RED << "Error while getting file descriptor: " << strerror(errno) << RESET << std::endl;
        //return error page, end connection
    }

    fstat(fd, &file_stat);
    this->_event.setFileSize(file_stat.st_size);
    this->_event.setEventSubStatus(WritingResponseHeaders);
}


void EventHandler::process_event() {
    if (this->_event.getEventStatus() == Reading) {

        switch (this->_event.getEventSubStatus()) {
            case ReadingRequest: _request.read_request();
            case ParsingRequest: _request.parse_request();
                break;
            default:
                std::cerr << RED << "Invalid Reading Event Sub Status" << RESET << std::endl;
                break;
        }
    }

    if (this->_event.getEventStatus() == Writing) {        switch (this->_event.getEventSubStatus()) {
            case OpeningFile: open_file();
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

    std::cout << "Event Status: " << status << std::endl;
    std::cout << "Event Sub Status: " << sub_status << std::endl;
}

const Event &EventHandler::getEvent() const {
    return _event;
}

void EventHandler::setEvent(const Event &event) {
    this->_event = event;
}

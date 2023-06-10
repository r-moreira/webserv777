//
// Created by rmore on 10/04/2023.
//

#include "../../includes/io/File.h"

File::File(Event &event): _event(event) {}

File::~File() {}

void File::create_file() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isFileOpened()) return;

    Logger::debug("Creating file: " + this->_event.getFilePath());

    FILE *fptr;
    fptr = fopen(this->_event.getFilePath().c_str(), "wb");
    if (fptr == NULL) {
        Logger::error("Error while creating file: " + this->_event.getFilePath() + " " + strerror(errno));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    this->_event.setFile(fptr);
    this->_event.setFileOpened(true);
}

void File::delete_file() {
    if (ErrorState::is_error_state(this->_event)) return;

    Logger::debug("Deleting file: " + this->_event.getFilePath());

    //check if file exists
    if (access(this->_event.getFilePath().c_str(), F_OK) == -1) {
        Logger::warning("Deleting file - NOT FOUND: " + this->_event.getFilePath() + " " + strerror(errno));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::NOT_FOUND);
        return;
    }

    if (remove(this->_event.getFilePath().c_str()) != 0) {
        Logger::error("Error while deleting file: " + this->_event.getFilePath() + " " + strerror(errno));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    Logger::debug("File deleted: " + this->_event.getFilePath());
    _event.setEventStatus(Event::Status::Ended);
}

void File::open_file() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isFileOpened()) return;

    Logger::debug("Opening file: " + this->_event.getFilePath());

    if (this->_event.getFile() == NULL) {

        FILE *fptr;
        fptr = fopen(this->_event.getFilePath().c_str(), "rb");
        if (fptr == NULL) {
            Logger::error("Error while opening file: " + this->_event.getFilePath() + " " + strerror(errno));
            ErrorState::throw_error_state(this->_event, Event::HttpStatus::NOT_FOUND);
            return;
        }
        this->_event.setFile(fptr);
        this->_event.setFileReadBytes(0);
    }

    struct stat file_stat;
    int fd = fileno(this->_event.getFile());
    if (fd < 1) {
        Logger::error("Error while getting file descriptor: " + this->_event.getFilePath() + " " + strerror(errno));
        ErrorState::throw_error_state(this->_event, Event::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    fstat(fd, &file_stat);
    this->_event.setFileSize(file_stat.st_size);

    this->_event.setFileOpened(true);
    Logger::trace("File opened: " + this->_event.getFilePath() + " and has " + ITOSTR(this->_event.getFileSize()) + " bytes");
  }


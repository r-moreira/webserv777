//
// Created by rmore on 10/04/2023.
//

#include "../../includes/io/File.h"

File::File(Event &event): _event(event) {}

File::~File() {}

void File::create_file() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isFileOpened()) return;

    std::cout << "Creating file: " << this->_event.getFilePath() << std::endl;

    FILE *fptr;
    fptr = fopen(this->_event.getFilePath().c_str(), "wb");
    if (fptr == NULL) {
        std::cerr << RED << "Error while creating file: " << this->_event.getFilePath() << " " << strerror(errno) << RESET << std::endl;

        //Não funciona
        ErrorState::throw_error_state(this->_event, Event::INTERNAL_SERVER_ERROR);
        return;
    }
    this->_event.setFile(fptr);

    this->_event.setFileOpened(true);
}

void File::delete_file() {
    if (ErrorState::is_error_state(this->_event)) return;

    std::cout << "Deleting file: " << this->_event.getFilePath() << std::endl; //TODO: Adicionar o file path

    //check if file exists
    if (access(this->_event.getFilePath().c_str(), F_OK) == -1) {
        std::cerr << MAGENTA << "Deleting file - NOT FOUND: " << this->_event.getFilePath() << " " << strerror(errno) << RESET << std::endl;

        ErrorState::throw_error_state(this->_event, Event::NOT_FOUND);
        return;
    }

    if (remove(this->_event.getFilePath().c_str()) != 0) {
        std::cerr << RED << "Error while deleting file: " << this->_event.getFilePath() << " " << strerror(errno) << RESET << std::endl;

        ErrorState::throw_error_state(this->_event, Event::INTERNAL_SERVER_ERROR);
        return;
    }

    std::cout << "File deleted: " << this->_event.getFilePath() << std::endl;
    _event.setEventStatus(Event::Ended);
}

void File::open_file() {
    if (ErrorState::is_error_state(this->_event) || this->_event.isFileOpened()) return;

    std::cout << "Opening file: " << this->_event.getFilePath() << std::endl;

    if (this->_event.getFile() == NULL) {

        FILE *fptr;
        fptr = fopen(this->_event.getFilePath().c_str(), "rb");
        if (fptr == NULL) {
            std::cerr << RED << "Error while opening file: " << this->_event.getFilePath() << " " << strerror(errno) << RESET << std::endl;

            ErrorState::throw_error_state(this->_event, Event::NOT_FOUND);
            return;
        }
        this->_event.setFile(fptr);
        this->_event.setFileReadBytes(0);
    }

    struct stat file_stat;
    int fd = fileno(this->_event.getFile());
    if (fd < 1) {
        std::cerr << RED << "Error while getting file descriptor: " << strerror(errno) << RESET << std::endl;
        ErrorState::throw_error_state(this->_event, Event::INTERNAL_SERVER_ERROR);
        return;
    }

    fstat(fd, &file_stat);
    this->_event.setFileSize(file_stat.st_size);

    this->_event.setFileOpened(true);
    std::cout << CYAN << "Successfully opened file: " << this->_event.getFilePath() << RESET << std::endl;
}


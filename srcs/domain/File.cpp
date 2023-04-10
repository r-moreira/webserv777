//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/File.h"

File::File(Event &event): _event(event) {}

File::~File() {}

void File::open_file() {
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

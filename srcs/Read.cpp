#include "../includes/Read.hpp"

FT::Read::Read(int fd) {
    is_read = BUFFER_SIZE;
    readig_fd(fd);
}

void FT::Read::readig_fd(int fd) {
    while (is_read == BUFFER_SIZE)
    {
        is_read = read(fd, buffer, BUFFER_SIZE);
        std::string aux(buffer, sizeof(buffer));
        text = text + aux;
    }
}

std::string FT::Read::get_text() {
    return text;
}
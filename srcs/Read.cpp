#include "../includes/Read.hpp"

FT::Read::Read(int fd) {
    is_read = BUFFER_SIZE;
    size = 0;
    readig_fd(fd);
}

void FT::Read::readig_fd(int fd) {
    while (is_read == BUFFER_SIZE)
    {
        is_read = read(fd, buffer, BUFFER_SIZE);
        std::string aux(buffer, sizeof(buffer));
        text = text + aux;
        size += is_read;
    }
}

std::string FT::Read::getText() {
    return text;
}

int FT::Read::getSize() {
    return size;
}
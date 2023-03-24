#include "../includes/Reading.hpp"

FT::Reading::Reading(int fd) {
    is_read = BUFFER_SIZE;
    readig_fd(fd);
}

void FT::Reading::readig_fd(int fd) {
    while (is_read == BUFFER_SIZE)
    {
        is_read = read(fd, buffer, BUFFER_SIZE);
        text = text + buffer;
    }
}

std::string FT::Reading::get_text() {
    return text;
}
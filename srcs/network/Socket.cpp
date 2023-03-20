//
// Created by rmore on 15/03/2023.
//

#include "../../includes/network/Socket.h"

Socket::Socket() {
    _addr = new struct sockaddr_in;
}

Socket::~Socket() {
    delete _addr;
}

Socket &Socket::operator=(const Socket &rhs) {
    delete _addr;

    if (this != &rhs) {
        _addr = new struct sockaddr_in;
        _fd = rhs._fd;
        _addr = rhs._addr;
    }
    return *this;
}

int Socket::getFd() const {
    return _fd;
}

void Socket::setFd(int fd) {
    _fd = fd;
}

sockaddr_in *Socket::getAddr() const {
    return _addr;
}

void Socket::setAddr(sockaddr_in *addr) {
    _addr = addr;
}

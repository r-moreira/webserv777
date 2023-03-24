//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_SOCKET_H
#define WEBSERV_SOCKET_H

#include "../webserv.h"

class Socket {

private:
    int _fd;
    struct sockaddr_in *_addr;

public:
    Socket();
    ~Socket();

    int getFd() const;

    void setFd(int fd);

    sockaddr_in *getAddr() const;

    void setAddr(sockaddr_in *addr);

    Socket &operator=(const Socket &rhs);

};


#endif //WEBSERV_SOCKET_H

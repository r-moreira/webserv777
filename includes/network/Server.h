//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_SERVER_H
#define WEBSERV_SERVER_H

#include "./Socket.h"

class ServerBuilder;

class Server {

private:
    std::string _name;
    int _port;
    int _fd;
    long _max_body_size;

public:
    Server();
    ~Server();

    friend class ServerBuilder;

    static ServerBuilder build();

    std::string getName() const;

    int getPort() const;

    int getFd() const;

    int getMaxBodySize() const;

    void setMaxBodySize(int maxBodySize);
};

std::ostream& operator << (std::ostream& os, const Server& obj);

#endif //WEBSERV_SERVER_H

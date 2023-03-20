//
// Created by rmore on 20/03/2023.
//

#ifndef WEBSERV_REQUEST_H
#define WEBSERV_REQUEST_H


#include <string>
#include <vector>
#include <sstream>


struct Request {
    Request()
            : versionMajor(0), versionMinor(0), keepAlive(false) {}

    struct HeaderItem {
        std::string name;
        std::string value;
    };

    std::string method;
    std::string uri;
    int versionMajor;
    int versionMinor;
    std::vector<HeaderItem> headers;
    std::vector<char> content;
    bool keepAlive;

    std::string inspect() const {
        std::stringstream stream;
        stream << method << " " << uri << " HTTP/"
               << versionMajor << "." << versionMinor << "\n";

        for (std::vector<Request::HeaderItem>::const_iterator it = headers.begin();
             it != headers.end(); ++it) {
            stream << it->name << ": " << it->value << "\n";
        }

        std::string data(content.begin(), content.end());
        stream << data << "\n";
        stream << "+ keep-alive: " << keepAlive << "\n";;
        return stream.str();
    }

    const std::string &getUri() const {
        return uri;
    }
};


#endif //WEBSERV_REQUEST_H

//
// Created by rmore on 20/03/2023.
//

#ifndef WEBSERV_RESPONSE_H
#define WEBSERV_RESPONSE_H


#include <string>
#include <vector>
#include <sstream>

struct Response {
    Response()
            : versionMajor(0), versionMinor(0), keepAlive(false), statusCode(0) {}

    struct HeaderItem {
        std::string name;
        std::string value;
    };

    int versionMajor;
    int versionMinor;
    std::vector<HeaderItem> headers;
    std::vector<char> content;
    bool keepAlive;

    unsigned int statusCode;
    std::string status;

    std::string inspect() const {
        std::stringstream stream;
        stream << "HTTP/" << versionMajor << "." << versionMinor
               << " " << statusCode << " " << status << "\n";

        for (std::vector<Response::HeaderItem>::const_iterator it = headers.begin();
             it != headers.end(); ++it) {
            stream << it->name << ": " << it->value << "\n";
        }

        std::string data(content.begin(), content.end());
        stream << data << "\n";
        return stream.str();
    }
};

#endif //WEBSERV_RESPONSE_H

#include "../../includes/ConfigParcer/ConfigParcer.hpp"

FT::ConfigParcer::ConfigParcer() {
    //fileContent = read file
    is_valide();
    serverParcer();
}

void FT::ConfigParcer::serverParcer() {
    std::string delimiter = "server";
    std::string endDelimiter = "}";
    int lenDelimiter = delimiter.size();

    if (fileContent.find(delimiter) == fileContent.size()) {
        return ;
    }
    std::string server = fileContent.substr(fileContent.find(delimiter) + lenDelimiter, fileContent.find(endDelimiter) - lenDelimiter);
    fileContent = fileContent.substr(0, server.size());
    serverParcerAtributes(server);
    serverParcer();
}

int FT::ConfigParcer::is_valide() {
    std::string delimiter = "server";
    int openQuotes = 0;

    if(fileContent.find(delimiter) == fileContent.size()){
        return 0;
    }
    for (int i = 0; i < fileContent.size(); i++) {
        if (fileContent[i] == '{') {
            openQuotes++;
        }
        else if (fileContent[i] == '}') {
            openQuotes--;
        }
    }
    if (openQuotes != 0) {
        return 0;
    }
    return 1;
}

void FT::ConfigParcer::serverParcerAtributes(std::string serverString) {
    ServerType server;
    //parcer atributes
    servers.push_back(server);
}
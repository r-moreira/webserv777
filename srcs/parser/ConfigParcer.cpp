#include "../../includes/ConfigParcer/ConfigParcer.hpp"

FT::ConfigParcer::ConfigParcer(std::string fileName) {
    read_file(fileName);
    is_valide();
    serverParcer();
}

void FT::ConfigParcer::read_file(std::string fileName) {
    int fd;
    fd = open(fileName.data(), O_RDONLY, 0644);

    char buffer[READ_BUFFER_SIZE] = {};

    long bytes_read = read(fd, buffer, READ_BUFFER_SIZE);

    if (bytes_read == -1) {
        std::cerr << RED << "Error while reading server config file " << strerror(errno) << RESET << std::endl;
    }
    fileContent = buffer;
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
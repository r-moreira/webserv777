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
    
void FT::ConfigParcer::parcerPort(ServerType server) {
    std::string delimiter = "listen ";
    std::string endDelimiter = "\n";
    server.port = fileContent.substr(fileContent.find(delimiter), fileContent.find(endDelimiter));
}

void FT::ConfigParcer::serverName(ServerType server) {
    std::string delimiter = "server_name ";
    std::string endDelimiter = "\n";
    server.serverName = fileContent.substr(fileContent.find(delimiter), fileContent.find(endDelimiter));
}

void FT::ConfigParcer::root(ServerType server) {
    std::string delimiter = "root ";
    std::string endDelimiter = "\n";
    server.root = fileContent.substr(fileContent.find(delimiter), fileContent.find(endDelimiter));
}

void FT::ConfigParcer::index(ServerType server) {
    std::string delimiter = "index ";
    std::string endDelimiter = "\n";
    server.index = spliteString(fileContent.substr(fileContent.find(delimiter), fileContent.find(endDelimiter)));
}

void FT::ConfigParcer::errorPage(ServerType server) {
    std::string delimiter = "error_page ";
    std::string endDelimiter = "\n";
    server.errorPage = spliteString(fileContent.substr(fileContent.find(delimiter), fileContent.find(endDelimiter)));
}

void FT::ConfigParcer::maxBodySize(ServerType server) {
    std::string delimiter = "client_max_body_size ";
    std::string endDelimiter = "\n";
    server.maxBodySize = atoi(fileContent.substr(fileContent.find(delimiter), fileContent.find(endDelimiter)).data());
}

void FT::ConfigParcer::parcerLocation(ServerType server) {
    // std::string delimiter = "location ";
    // std::string endDelimiter = "\n";
    // std::vector<std::string> location = spliteString(fileContent.substr(fileContent.find(delimiter), fileContent.find(endDelimiter)));
}

std::vector<std::string> FT::ConfigParcer::spliteString(std::string str) {
    std::istringstream iss(str);
    std::vector<std::string> words;

    std::string word;
    while (std::getline(iss, word, ' ')) {
        words.push_back(word);
    }
    return words;
}
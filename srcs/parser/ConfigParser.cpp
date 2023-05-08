#include "../../includes/parser/ConfigParser.hpp"

FT::ConfigParser::ConfigParser(std::string fileName) {
    read_file(fileName);
    is_valide();
    serverParcer();
}

void FT::ConfigParser::read_file(std::string fileName) {
    int fd;
    fd = open(fileName.data(), O_RDONLY, 0644);

    char buffer[READ_BUFFER_SIZE] = {};

    long bytes_read = read(fd, buffer, READ_BUFFER_SIZE);

    if (bytes_read == -1) {
        std::cerr << RED << "Error while reading server config file " << strerror(errno) << RESET << std::endl;
    }
    fileContent = buffer;
}

void FT::ConfigParser::serverParcer() {
    std::string delimiter = "server {";
    int serverCount = 0;
    ServerType* server = new ServerType();
    std::istringstream iss(fileContent);

    std::string word;
    while (std::getline(iss, word, '\n')) {
        if (word.find(delimiter) != std::string::npos) {
            serverCount++;
            if (serverLocationAtribute) {
                server->locations.push_back(serverLocationAtribute);
            }
            serverLocationCount = 0;
            serverLocationAtribute = NULL;
        }
        else if (serverCount) {
            if (serverCount > 1) {
                servers.push_back(server);
                serverCount--;
                server = new ServerType();
            }
            parcerPort(server, word);
            serverName(server, word);
            root(server, word);
            index(server, word);
            errorPage(server, word);
            maxBodySize(server, word);
            parcerLocation(server, word);
        }
    }
    if (serverCount == 1) {
        servers.push_back(server);
    }
}

int FT::ConfigParser::is_valide() {
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
   
void FT::ConfigParser::parcerPort(ServerType *server, std::string atribute) {
    std::string delimiter = "port ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->port = atoi(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n).data());
    }
}

void FT::ConfigParser::serverName(ServerType *server, std::string atribute) {
    std::string delimiter = "server_name ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->serverName = atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n);
    }
}

void FT::ConfigParser::root(ServerType *server, std::string atribute) {
    std::string delimiter = "root ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->root = atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n);
    }
}

void FT::ConfigParser::index(ServerType *server, std::string atribute) {
    std::string delimiter = "index ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->index = spliteString(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n));
    }
}

void FT::ConfigParser::errorPage(ServerType *server, std::string atribute) {
    std::string delimiter = "error_page ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->errorPage = spliteString(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n));
    }
    
}

void FT::ConfigParser::maxBodySize(ServerType *server, std::string atribute) {
    std::string delimiter = "client_max_body_size ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->maxBodySize = atoi(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n).data());
    }
}

void FT::ConfigParser::parcerLocation(ServerType* server, std::string atribute) {
    std::string delimiter = "location ";
    int isContains = contains(delimiter, atribute);
    if (isContains || serverLocationCount) {
        if (isContains) {
            if (serverLocationAtribute) {
                server->locations.push_back(serverLocationAtribute);
                serverLocationCount--;
            }
            serverLocationAtribute = new LocationParser();
            serverLocationCount++;
        }
        serverLocationAtribute->addNewAtribute(atribute);
    }
}

std::vector<std::string> FT::ConfigParser::spliteString(std::string str) {
    std::istringstream iss(str);
    std::vector<std::string> words;

    std::string word;
    while (std::getline(iss, word, ' ')) {
        words.push_back(word);
    }
    return words;
}

int FT::ConfigParser::contains(std::string delimiter, std::string str) {
    return str.find(delimiter) != std::string::npos ? 1 : 0;
}

std::vector<FT::ServerType *> FT::ConfigParser::getServers() {
    return servers;
}

int FT::ConfigParser::getHowMuchServers() {
    return servers.size();
}

FT::ServerType & FT::ConfigParser::operator[](int i) {
    return *(servers[i]);
}
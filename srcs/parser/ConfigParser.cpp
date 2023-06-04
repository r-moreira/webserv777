#include "../../includes/parser/ConfigParser.hpp"

FT::ConfigParser::ConfigParser(std::string fileName): serverLocationCount(0), serverLocationAtribute(NULL) {
    std::string delimiter = "server {";
    int serverCount = 0;
    ServerType* server = new ServerType();
    std::ifstream iss(fileName.data());

    std::string word;
    while (std::getline(iss, word, '\n')) {
        if (!is_comment(word)) {
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
                parcerLimitExcept(server, word);
                parcerDirPage(server, word);
                parcerUploadPath(server, word);
                parcerAutoIndex(server, word);
            }
        }
    }
    if (serverCount == 1) {
        servers.push_back(server);
    }
}

void FT::ConfigParser::parcerPort(ServerType *server, std::string atribute) {
    std::string delimiter = "listen ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->port = atoi(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n).data());
    }
}

void FT::ConfigParser::serverName(ServerType *server, std::string atribute) {
    std::string delimiter = "_server_name ";
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
    std::string delimiter = "max_body_size ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->maxBodySize = std::atoi(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n).c_str());
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

int FT::ConfigParser::contains(std::string delimiter, std::string str) {
    return str.find(delimiter) != std::string::npos ? 1 : 0;
}

std::vector<FT::ServerType *> FT::ConfigParser::getServers() {
    return servers;
}

FT::ServerType & FT::ConfigParser::operator[](int i) {
    return *(servers[i]);
}

void FT::ConfigParser::parcerLimitExcept(ServerType *server, std::string atribute) {
    std::string delimiter = "limit_except ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->limitExcept = spliteString(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n));
    }
}

void FT::ConfigParser::parcerDirPage(ServerType *server, std::string atribute) {
    std::string delimiter = "directory_page ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->directoryPage = atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n);
    }
}

int FT::ConfigParser::is_comment(std::string str) {
    std::string delimiter = "#";
    return contains(delimiter, str);
}

void FT::ConfigParser::parcerUploadPath(ServerType *server, std::string atribute) {
    std::string delimiter = "upload ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->uploadPath = atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n);
        server->uploadLock = true;
    }
}

void FT::ConfigParser::parcerAutoIndex(ServerType *server, std::string atribute) {
    std::string delimiter = "auto_index ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->auto_index = atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n) == "on" ? true : false;
    }
}

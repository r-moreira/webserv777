#include "../../includes/parser/ConfigParser.h"

ConfigParser::ConfigParser(std::string fileName): serverLocationCount(0), serverLocationAtribute(NULL) {
    std::string delimiter = "server";
    int serverCount = 0;
    ServerType* server = new ServerType();
    std::ifstream iss(fileName.data());

    std::string word;
    while (std::getline(iss, word, '\n')) {
        word.find('\r') != std::string::npos ? word.replace(word.find('\r'), 1, "") : word;
        if (!is_comment(word)) {
            if (word == delimiter or word == (delimiter + " {")) {
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
                if (serverLocationCount == 0) {
                    setServerAtributes(server, word);
                }
                parcerLocation(server, word);
            }
        }
    }
    if (serverCount == 1) {
        servers.push_back(server);
    }
    if (serverLocationCount) {
        server->locations.push_back(serverLocationAtribute);
    }
}

void ConfigParser::setServerAtributes(ServerType *server, std::string atribute) {
    parcerPort(server, atribute);
    serverName(server, atribute);
    root(server, atribute);
    index(server, atribute);
    errorPage(server, atribute);
    maxBodySize(server, atribute);
    parcerLimitExcept(server, atribute);
    parcerDirPage(server, atribute);
    parcerUploadPath(server, atribute);
    parcerAutoIndex(server, atribute);
}

void ConfigParser::parcerPort(ServerType *server, std::string atribute) {
    std::string delimiter = "listen ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->port = atoi(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n).data());
    }
}

void ConfigParser::serverName(ServerType *server, std::string atribute) {
    std::string delimiter = "_server_name ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->serverName = atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n);
    }
}

void ConfigParser::root(ServerType *server, std::string atribute) {
    std::string delimiter = "root ";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->root = atribute.substr(atribute.find(delimiter) + n, atribute.size());
    }
}

void ConfigParser::index(ServerType *server, std::string atribute) {
    std::string delimiter = " index ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->index = spliteString(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n));
    }
}

void ConfigParser::errorPage(ServerType *server, std::string atribute) {
    std::string delimiter = "error_page ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->errorPage = spliteString(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n));
    }
    
}

void ConfigParser::maxBodySize(ServerType *server, std::string atribute) {
    std::string delimiter = "max_body_size ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->maxBodySize = std::atoi(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n).c_str());
    }
}

void ConfigParser::parcerLocation(ServerType* server, std::string atribute) {
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

int ConfigParser::contains(std::string delimiter, std::string str) {
    return str.find(delimiter) != std::string::npos ? 1 : 0;
}

std::vector<ConfigParser::ServerType *> ConfigParser::getServers() {
    return servers;
}

ConfigParser::ServerType & ConfigParser::operator[](int i) {
    return *(servers[i]);
}

void ConfigParser::parcerLimitExcept(ServerType *server, std::string atribute) {
    std::string delimiter = "limit_except ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->limitExcept = spliteString(atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n));
    }
}

void ConfigParser::parcerDirPage(ServerType *server, std::string atribute) {
    std::string delimiter = "directory_page ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->directoryPage = atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n);
    }
}

int ConfigParser::is_comment(std::string str) {
    std::string delimiter = "#";
    return contains(delimiter, str);
}

void ConfigParser::parcerUploadPath(ServerType *server, std::string atribute) {
    std::string delimiter = "upload_path ";
    int n = delimiter.size();

    if (contains(delimiter, atribute)) {
        server->uploadPath = atribute.substr(atribute.find(delimiter) + n, atribute.size());
        server->uploadLock = true;
    }
}

void ConfigParser::parcerAutoIndex(ServerType *server, std::string atribute) {
    std::string delimiter = "auto_index ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, atribute)) {
        server->auto_index = (atribute.substr(atribute.find(delimiter) + n, atribute.find(endDelimiter) - n) == "on" ? AutoIndexOption::ON : AutoIndexOption::OFF);
    }
}

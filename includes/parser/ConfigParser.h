#ifndef ConfigParser_hpp
#define ConfigParser_hpp

#include "../webserv.h"
#include "LocationParser.h"
#include "../config/AutoIndexOption.h"

class ConfigParser
{
public:
    struct ServerType {
        int port;
        std::string serverName;
        std::string root;
        std::vector<std::string> limitExcept;
        std::string directoryPage;
        std::vector<std::string> index;
        std::vector<std::string> errorPage;
        int maxBodySize;
        AutoIndexOption::option auto_index;
        std::vector<LocationParser*> locations;
        std::string uploadPath;
        bool uploadLock;
    };
private:
    int serverLocationCount;
    LocationParser *serverLocationAtribute;
    std::vector<ServerType *> servers;
    void parcerPort(ServerType* server, std::string atribute);
    void serverName(ServerType* server, std::string atribute);
    void root(ServerType* server, std::string atribute);
    void index(ServerType* server, std::string atribute);
    void errorPage(ServerType* server, std::string atribute);
    void maxBodySize(ServerType* server, std::string atribute);
    void parcerLocation(ServerType* server, std::string atribute);
    int contains(std::string delimiter, std::string str);
    void parcerLimitExcept(ServerType *server, std::string location);
    void parcerDirPage(ServerType *server, std::string atribute);
    int is_comment(std::string str);
    void parcerUploadPath(ServerType *server, std::string atribute);
    void parcerAutoIndex(ServerType *server, std::string atribute);
public:
    ConfigParser(std::string fileName);
    std::vector<ServerType *> getServers();
    ServerType & operator[](int i);
};
#endif
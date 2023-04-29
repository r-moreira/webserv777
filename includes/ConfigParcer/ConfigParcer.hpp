#ifndef ConfigParcer_hpp
#define ConfigParcer_hpp

#include "../webserv.h"
#include "Location.hpp"

#define READ_BUFFER_SIZE 8192

namespace FT {
struct ServerType {
    int port;
    std::string serverName;
    std::string root;
    std::vector<std::string> index;
    std::vector<std::string> errorPage;
    std::vector<std::string> cgi;
    int maxBodySize;
    bool auto_index;
    std::vector<Location*> locations;
};

class ConfigParcer
{
private:
    int serverLocationCount;
    Location *serverLocationAtribute;
    std::vector<ServerType *> servers;
    std::string fileContent;
    std::vector<std::string> spliteString(std::string str);
    void parcerPort(ServerType* server, std::string atribute);
    void read_file(std::string fileName);
    void serverName(ServerType* server, std::string atribute);
    void root(ServerType* server, std::string atribute);
    void index(ServerType* server, std::string atribute);
    void errorPage(ServerType* server, std::string atribute);
    void maxBodySize(ServerType* server, std::string atribute);
    void parcerLocation(ServerType* server, std::string atribute);
    int contains(std::string delimiter, std::string str);
    void serverParcer();
    int is_valide();
public:
    ConfigParcer(std::string fileName);
    std::vector<ServerType *> getServers();
    int getHowMuchServers();
    ServerType & operator[](int i);
};
}
#endif
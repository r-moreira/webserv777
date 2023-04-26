#ifndef ConfigParcer_hpp
#define ConfigParcer_hpp

#include "../../includes/webserv.h"

#define READ_BUFFER_SIZE 8192

namespace FT {

struct LocationType {
    std::string limitExcept;
    std::string root;
    std::string returnPage;
    bool cgi_lock;
};

struct ServerType {
    std::string port;
    std::string serverName;
    std::string root;
    std::vector<std::string> index;
    std::vector<std::string> errorPage;
    std::vector<std::string> cgi;
    int maxBodySize;
    bool auto_index;
    std::vector<LocationType> location;
};

class ConfigParcer
{
private:
    std::vector<ServerType> servers;
    std::string fileContent;
    std::vector<std::string> spliteString(std::string str);
    void parcerPort(ServerType server);
    void read_file(std::string fileName);
    void serverName(ServerType server);
    void root(ServerType server);
    void index(ServerType server);
    void errorPage(ServerType server);
    void maxBodySize(ServerType server);
    void parcerLocation(ServerType server);
public:
    ConfigParcer(std::string fileName);
    void serverParcer();
    void serverParcerAtributes(std::string server);
    int is_valide();
    ~ConfigParcer();
};
}
#endif
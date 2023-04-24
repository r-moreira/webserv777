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
    std::string **index;
    std::string **errorPage;
    std::string **cgi;
    bool auto_index;
    LocationType **location;
};

class ConfigParcer
{
private:
    std::vector<ServerType> servers;
    std::string fileContent;

    void read_file(std::string fileName);
public:
    ConfigParcer(std::string fileName);
    void serverParcer();
    void serverParcerAtributes(std::string server);
    int is_valide();
    ~ConfigParcer();
};
}
#endif
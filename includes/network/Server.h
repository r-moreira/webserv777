//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_SERVER_H
#define WEBSERV_SERVER_H

#include "./Location.h"
#include "./Socket.h"

class Server {

private:
    std::vector<std::string> _serverNames;
    Socket _socket;
    size_t _port;
    std::vector<Location> _locations;
    std::vector<std::pair<std::string, std::string> > _errorPages;
    std::string _uploadPath;
    std::vector<std::string> _mimeTypes;

public:
    std::string root;
    char **_env;
    int a;
    int en_handle;

    std::vector<std::string> &getmime_types();

    std::vector<std::pair<std::string, std::string> > _cgi;

    Server();

    ~Server();

    void init_MimeTypes();

    const std::vector<std::string> &getServerNames() const;

    void setServerNames(const std::vector<std::string> &serverNames);

    const Socket &getSocket() const;

    void setSocket(const Socket &socket);

    size_t getPort() const;

    void setPort(size_t port);

    const std::vector<Location> &getLocations() const;

    void setLocations(const std::vector<Location> &locations);

    const std::vector<std::pair<std::string, std::string> > &getErrorPages() const;

    void setErrorPages(const std::vector<std::pair<std::string, std::string> > &errorPages);

    const std::string &getUploadPath() const;

    void setUploadPath(const std::string &uploadPath);

    const std::vector<std::string> &getMimeTypes() const;

    void setMimeTypes(const std::vector<std::string> &mimeTypes);

    const std::string &getRoot() const;

    void setRoot(const std::string &root);

    char **getEnv() const;

    void setEnv(char **env);

    int getA() const;

    void setA(int a);

    int getEnHandle() const;

    void setEnHandle(int enHandle);

    const std::vector<std::pair<std::string, std::string> > &getCgi() const;

    void setCgi(const std::vector<std::pair<std::string, std::string> > &cgi);

};

#endif //WEBSERV_SERVER_H

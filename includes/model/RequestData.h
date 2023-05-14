//
// Created by rmore on 13/05/2023.
//

#ifndef WEBSERV_REQUESTDATA_H
#define WEBSERV_REQUESTDATA_H

#include "../webserv.h"

class RequestData {
public:
    struct HeaderItem {
        std::string name;
        std::string value;
    };

private:
    std::string _method;
    std::string _uri;
    int _version_major;
    int _version_minor;
    std::vector<RequestData::HeaderItem> _headers;
    std::vector<char> _content;
    bool _keep_alive;

public:
    RequestData();
    ~RequestData();

    //Fazer push back dos atributos membros privados da classe

    std::string inspect() const;

    void methodPushBack(char c);

    void uriPushBack(char c);

    void headerItemPushBackNameReserve(char  c);

    void headerItemPushBackValue(char  c);

    void headerItemPushBackName(char  c);

    void reserveContent(size_t size);

    void contentPushBack(char c);

    void setVersionMajor(int versionMajor);

    void setVersionMinor(int versionMinor);

    void setKeepAlive(bool keepAlive);

    const std::string &getMethod() const;

    const std::string &getUri() const;

    int getVersionMajor() const;

    int getVersionMinor() const;

    const std::vector<RequestData::HeaderItem> &getHeaders() const;

    const std::vector<char> &getContent() const;

    bool isKeepAlive() const;
};


#endif //WEBSERV_REQUESTDATA_H

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
    int _versionMajor;
    int _versionMinor;
    std::vector<RequestData::HeaderItem> _headers;
    std::vector<char> _content;
    bool keepAlive;

public:
    RequestData();
    ~RequestData();

    std::string inspect() const;

    const std::string &getMethod() const;

    void setMethod(const std::string &method);

    const std::string &getUri() const;

    void setUri(const std::string &uri);

    int getVersionMajor() const;

    void setVersionMajor(int versionMajor);

    int getVersionMinor() const;

    void setVersionMinor(int versionMinor);

    const std::vector<RequestData::HeaderItem> &getHeaders() const;

    void setHeaders(const std::vector<RequestData::HeaderItem> &headers);

    const std::vector<char> &getContent() const;

    void setContent(const std::vector<char> &content);

    bool isKeepAlive() const;

    void setKeepAlive(bool keepAlive);

};


#endif //WEBSERV_REQUESTDATA_H

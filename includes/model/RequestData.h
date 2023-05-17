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
    bool _is_file_upload;
    size_t _upload_file_size;
    std::string _boundary;
    std::string _content_disposition;
    std::string _upload_file_type;


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

    void contentDispositionPushBack(char c);

    void uploadFileTypePushBack(char c);

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

    bool isIsFileUpload() const;

    void setIsFileUpload(bool isFileUpload);

    size_t getFileUploadRemainingBytes() const;

    void setFileUploadRemainingBytes(size_t remainingBytes);

    const std::string &getBoundary() const;

    void setBoundary(const std::string &boundary);

    const std::string &getContentDisposition() const;

    void setContentDisposition(const std::string &contentDisposition);

    const std::string &getUploadFileType() const;

    void setUploadFileType(const std::string &uploadFileType);

};


#endif //WEBSERV_REQUESTDATA_H

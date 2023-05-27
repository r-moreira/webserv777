//
// Created by rmore on 25/05/2023.
//

#ifndef WEBSERV_ENVIRONMENT_H
#define WEBSERV_ENVIRONMENT_H

#include "../../includes/webserv.h"
#include "../../includes/model/RequestData.h"
#include "../domain/Event.h"


class Environment {
public:
    static int const ENV_VARIABLES_SIZE = 11;

private:
    std::string _server_name;
    std::string _server_protocol;
    std::string _server_port;
    std::string _request_method;
    std::string _path_info;
    std::string _path_translated;
    std::string _query_string;
    std::string _remote_host;
    std::string _content_type;
    std::string _content_length;
    std::string _http_user_agent;

public:
    Environment();

    ~Environment();

    char* const* getCgiEnvp(Event &event);

    static void freeCgiEnvp(char *const *envp);

private:
    void setServerNameValue(const std::string& value);

    void setServerProtocolValue(const std::string& value);

    void setServerPortValue(const std::string& value);

    void setRequestMethodValue(const std::string& value);

    void setPathInfoValue(const std::string& value);

    void setPathTranslatedValue(const std::string& value);

    void setQueryStringValue(const std::string& value);

    void setRemoteHostValue(const std::string& value);

    void setContentTypeValue(const std::string& value);

    void setContentLengthValue(const std::string& value);

    void setHttpUserAgentValue(const std::string& value);
};


#endif //WEBSERV_ENVIRONMENT_H

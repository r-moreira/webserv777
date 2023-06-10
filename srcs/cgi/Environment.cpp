//
// Created by rmore on 25/05/2023.
//

#include "../../includes/cgi/Environment.hpp"

Environment::Environment() {}

char *const *Environment::getCgiEnvp(Event &event) {
    this->_server_name = "SERVER_NAME=";
    this->_server_protocol = "SERVER_PROTOCOL=";
    this->_server_port = "SERVER_PORT=";
    this->_request_method = "REQUEST_METHOD=";
    this->_path_info = "PATH_INFO=";
    this->_path_translated = "PATH_TRANSLATED=";
    this->_query_string = "QUERY_STRING=";
    this->_remote_host = "REMOTE_HOST=";
    this->_content_type = "CONTENT_TYPE=";
    this->_content_length = "CONTENT_LENGTH=";
    this->_http_user_agent = "HTTP_USER_AGENT=";

    this->setServerNameValue(event.getServer().getName());
    this->setServerProtocolValue("HTTP/1.1");
    this->setServerPortValue(ITOSTR(event.getServer().getPort()));
    this->setRequestMethodValue(event.getRequest().getMethod());
    this->setPathInfoValue(event.getRequest().getUri());
    this->setPathTranslatedValue(event.getLocation().getCgiPath());
    this->setQueryStringValue(
            event.getRequest().getUri().find('?') != std::string::npos
            ? event.getRequest().getUri().substr(event.getRequest().getUri().find('?') + 1)
            : "");

    std::vector<RequestData::HeaderItem> headers = event.getRequest().getHeaders();

    for (std::vector<RequestData::HeaderItem>::iterator it = headers.begin(); it != headers.end(); it++) {
        if (it->name == "User-Agent") this->setHttpUserAgentValue(it->value);
        if (it->name == "Host") this->setRemoteHostValue(it->value);
        if (it->name == "Content-Type") this->setContentTypeValue(it->value);
        if (it->name == "Content-Length") this->setContentLengthValue(it->value);
    }

    char** envp;

    envp = (char **)malloc(sizeof (char*) * (ENV_VARIABLES_SIZE + 1));
    envp[0] = strdup(this->_server_name.c_str());
    envp[1] = strdup(this->_server_protocol.c_str());
    envp[2] = strdup(this->_server_port.c_str());
    envp[3] = strdup(this->_request_method.c_str());
    envp[4] = strdup(this->_path_info.c_str());
    envp[5] = strdup(this->_path_translated.c_str());
    envp[6] = strdup(this->_query_string.c_str());
    envp[7] = strdup(this->_remote_host.c_str());
    envp[8] = strdup(this->_content_type.c_str());
    envp[9] = strdup(this->_content_length.c_str());
    envp[10] = strdup(this->_http_user_agent.c_str());
    envp[11] = NULL;

    return envp;
}

void Environment::freeCgiEnvp(char * const* envp) {
    for (int i = 0; envp[ENV_VARIABLES_SIZE]; i++) {
        free(envp[i]);
    }
    free((char*)envp);
}

Environment::~Environment() {}

void Environment::setServerNameValue(const std::string& value) {
    this->_server_name.append(value);
}

void Environment::setServerProtocolValue(const std::string& value) {
    this->_server_protocol.append(value);
}

void Environment::setServerPortValue(const std::string& value) {
    this->_server_port.append(value);
}

void Environment::setRequestMethodValue(const std::string& value) {
    this->_request_method.append(value);
}

void Environment::setPathInfoValue(const std::string& value) {
    this->_path_info.append(value);
}

void Environment::setPathTranslatedValue(const std::string& value) {
    this->_path_translated.append(value);
}

void Environment::setQueryStringValue(const std::string& value) {
    this->_query_string.append(value);
}

void Environment::setRemoteHostValue(const std::string& value) {
    this->_remote_host.append(value);
}

void Environment::setContentTypeValue(const std::string& value) {
    this->_content_type.append(value);
}

void Environment::setContentLengthValue(const std::string& value) {
    this->_content_length.append(value);
}

void Environment::setHttpUserAgentValue(const std::string& value) {
    this->_http_user_agent.append(value);
}


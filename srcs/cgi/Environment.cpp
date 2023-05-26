//
// Created by rmore on 25/05/2023.
//

#include "../../includes/cgi/Environment.h"

Environment::Environment(){
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
}

Environment::~Environment(){}

void Environment::setupCGIEnvironment(Event &event) {
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
        if (it->name == "Host") this->setRequestMethodValue(it->value);
        if (it->name == "Content-Type") this->setContentTypeValue(it->value);
        if (it->name == "Content-Length") this->setContentLengthValue(it->value);
    }

}

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


char *Environment::_convert(const std::string & s)
{
    char *pc = new char[s.size()+1];
    std::strcpy(pc, s.c_str());
    return pc;
}

char **Environment::getCgiEnvp() {
    std::vector<std::string> vstring;
    vstring.push_back(this->_server_name);
    vstring.push_back(this->_server_protocol);
    vstring.push_back(this->_server_port);
    vstring.push_back(this->_request_method);
    vstring.push_back(this->_path_info);
    vstring.push_back(this->_path_translated);
    vstring.push_back(this->_query_string);
    vstring.push_back(this->_remote_host);
    vstring.push_back(this->_content_type);
    vstring.push_back(this->_content_length);
    vstring.push_back(this->_http_user_agent);

    std::vector<char *> vchar;
    std::transform(vstring.begin(), vstring.end(), std::back_inserter(vchar), _convert);

    char **envs = new char *[vchar.size() + 1];
    std::copy(vchar.begin(), vchar.end(), envs);

    return envs;
}


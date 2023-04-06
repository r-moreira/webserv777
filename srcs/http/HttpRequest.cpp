#include "../../includes/http/HttpRequest.hpp"

FT::HttpRequest::HttpRequest(int fd): Read(fd) {
    http_parser(getText());
}

std::string FT::HttpRequest::get_body() {
    return body;
}

std::string FT::HttpRequest::get_header() {
    return header;
}

std::string FT::HttpRequest::get_method() {
    return method;
}

std::string FT::HttpRequest::get_host() {
    return host;
}

bool FT::HttpRequest::isValidRequest() {
    std::vector<std::string>httpRequests = {"POST", "DELETE", "PUT", "GET"};
    for (int i = 0; i < httpRequests.size(); i++) {
        if (httpRequests[i] == method) {
            return true;
        }
    }
    return false;
}

void FT::HttpRequest::http_parser(std::string crudeRequest) {
    method_parser(crudeRequest);
    if (isValidRequest()) {
        header_parser(crudeRequest);
        host_parser(header);
        if (method != "GET") {
            body_parser(crudeRequest);
        }
    }
}

void FT::HttpRequest::header_parser(std::string request) {
    std::string delimiter = "Content-Length"; 
    header = request.substr(0, request.find(delimiter));
}

void FT::HttpRequest::host_parser(std::string req) {
    std::string delimiter = "Host: ";
    std::string endDelimiter = "\n";
    int lenDelimiter = delimiter.size();
    // if (find(delimiter) != size)
    host = req.substr(req.find(delimiter) + lenDelimiter, req.find(endDelimiter) - lenDelimiter);
}

void FT::HttpRequest::method_parser(std::string req) {
    std::string delimiter = " "; 
    method = req.substr(0, req.find(delimiter));
}

void FT::HttpRequest::body_parser(std::string req) {
    std::string aux = req.substr(header.size(), req.size());
    int delimiter = aux.find("{");
    int endDelimiter = aux.find("}"); 

    body = aux.substr(delimiter, (endDelimiter + 1) - delimiter);
}
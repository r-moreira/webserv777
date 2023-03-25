#include "../../includes/http/HttpRequest.hpp"

FT::HttpRequest::HttpRequest(std::string crudeRequest) {
    http_parser(crudeRequest);
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

void FT::HttpRequest::http_parser(std::string crudeRequest) {
    header_parser(crudeRequest);
    method_parser(header);
    host_parser(header);
    if (method != "GET") {
        body_parser(crudeRequest);
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
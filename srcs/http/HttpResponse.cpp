#include "../../includes/http/HttpResponse.hpp"

FT::HttpResponse::HttpResponse(HttpRequest *request) {}

std::string FT::HttpResponse::get_body() {
    return body;
}

std::string FT::HttpResponse::get_header() {
    return header;
}

std::string FT::HttpResponse::get_statusCode() {
    return statusCode;
}
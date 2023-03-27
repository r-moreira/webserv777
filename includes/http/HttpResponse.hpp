#ifndef HttpResponse_hpp
#define HttpResponse_hpp
#include <iostream>
#include "HttpRequest.hpp"

namespace FT {
class HttpResponse
{
public:
    HttpResponse(HttpRequest *request);
    ~HttpResponse();
    std::string get_body();
    std::string get_header();
    std::string get_statusCode();
private:
    std::string body;
    std::string header;
    std::string statusCode;
};
}
#endif
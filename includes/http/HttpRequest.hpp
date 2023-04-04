#ifndef HttpRequest_hpp
#define HttpRequest_hpp
#include <iostream>
#include <vector>
#include "Http.hpp"

namespace FT {
class HttpRequest: Http
{
public:
    HttpRequest(int fd);
    std::string get_body();
    std::string get_header();
    std::string get_method();
    std::string get_host();
private:
    std::string body;
    std::string header;
    std::string method;
    std::string host;
    bool isValidRequest();
    void header_parser(std::string request);
    void host_parser(std::string req);
    void method_parser(std::string req);
    void body_parser(std::string req);
    void http_parser(std::string crudeRequest);
};
}
#endif
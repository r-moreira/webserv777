#ifndef HttpResponse_hpp
#define HttpResponse_hpp
#include <iostream>
#include <fcntl.h>
#include <ctime>

#include "HttpRequest.hpp"
#include "../Read.hpp"

namespace FT {
class HttpResponse
{
public:
    HttpResponse(HttpRequest *request);
    ~HttpResponse();
    std::string get_response();
    std::string get_statusCode();
private:
    void make_header(int responseSize);
    int open_file(std::string fileName);
    void date_generate();
    Read *data;
    std::string response;
    std::string statusCode;
    std::string header;
    std::string date;
};
}
#endif
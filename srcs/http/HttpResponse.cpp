#include "../../includes/http/HttpResponse.hpp"

FT::HttpResponse::HttpResponse(HttpRequest *request) {
    data = new Read(open_file("public/index.html"));
    statusCode = "200";
    make_header(data->getSize());
    response = header + data->getText();
}

FT::HttpResponse::~HttpResponse() {
    delete data;
}

int FT::HttpResponse::open_file(std::string fileName){
    return open(fileName.data(), O_RDONLY, 0644);
}

std::string FT::HttpResponse::get_response() {
    return response;
}

std::string FT::HttpResponse::get_statusCode() {
    return statusCode;
}

void FT::HttpResponse::make_header(int responseSize) {
    // std::string content_types[] = {
    //     "Content-Type: text/css\r\n\r\n",
    //     "Content-Type: text/javascript\r\n\r\n",
    //     "Content-Type: application/json\r\n\r\n",
    //     "Content-Type: image/png\r\n\r\n",
    //     "Content-Type: application/x-httpd-php\r\n\r\n",
    //     "Content-Type: application/rtf\r\n\r\n",
    //     "Content-Type: image/svg+xml\r\n\r\n",
    //     "Content-Type: text/plain\r\n\r\n",
    //     "Content-Type: text/html\r\n\r\n",
    //     "Content-Type: image/jpeg\r\n\r\n",
    // };
    std::string str = "HTTP/1.1 " + statusCode + " OK\n" + "Content-Type: text/html\r\n\r\n";
    str = str + "Content-Length: " + std::to_string(responseSize) + "\r\n";
    header = str;
}
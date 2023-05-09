#ifndef HttpError_hpp
#define HttpError_hpp
#include <iostream>

class HttpError
{
private:
    std::string html;
    void _htmlGenerate(std::string imagePatch, std::string errorCode);
public:
    HttpError();
    std::string getError400();
    std::string getError403();
    std::string getError404();
    std::string getError405();
    std::string getError413();
    std::string getError500();
    std::string getError501();
};

#endif
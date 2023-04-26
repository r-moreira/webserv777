#ifndef Location_hpp
#define Location_hpp
#include <iostream>
#include <vector>

class Location
{
private:
    std::string endPoint;
    std::string limitExcept;
    std::string root;
    std::string returnPage;
    bool cgi_lock;
public:
    Location(std::string location);
    void parcerLimitExcept(std::string location);
    void parcerRoot(std::string location);
    void parcerReturnPage(std::string location);
    void parcerCgiLock(std::string location);
    void parcerEndPoint(std::string location);
    std::string getLimitExcept();
    std::string getRoot();
    std::string getReturnPage();
    std::string getEndPoint();
    bool getCgiLock();
};

#endif
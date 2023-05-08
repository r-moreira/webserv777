#ifndef LocationParser_hpp
#define LocationParser_hpp
#include <iostream>
#include <vector>

class LocationParser
{
private:
    std::string endPoint;
    std::string limitExcept;
    std::string root;
    std::string returnPage;
    bool cgi_lock;
    void parcerLimitExcept(std::string location);
    void parcerRoot(std::string location);
    void parcerReturnPage(std::string location);
    void parcerCgiLock(std::string location);
    void parcerEndPoint(std::string location);
    int contains(std::string delimiter, std::string str);
public:
    LocationParser();
    void addNewAtribute(std::string str);
    std::string getLimitExcept();
    std::string getRoot();
    std::string getReturnPage();
    std::string getEndPoint();
    bool getCgiLock();
};

#endif
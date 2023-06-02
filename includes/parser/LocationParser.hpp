#ifndef LocationParser_hpp
#define LocationParser_hpp
#include <iostream>
#include <vector>

class LocationParser
{
private:
    std::string endPoint;
    std::vector<std::string> limitExcept;
    std::string root;
    std::string redirect;
    std::string directoryPage;
    std::vector<std::string> errorPage;
    int maxBodySize;
    bool autoIndex;
    bool cgi_lock;
    void parcerLimitExcept(std::string location);
    void parcerRoot(std::string location);
    void parcerRedirect(std::string location);
    void parcerCgiLock(std::string location);
    void parcerEndPoint(std::string location);
    void parcerDirPage(std::string location);
    void parcerErrorPage(std::string location);
    int contains(std::string delimiter, std::string str);
public:
    LocationParser();
    void addNewAtribute(std::string str);
    std::vector<std::string> getLimitExcept();
    std::string getRoot();
    std::string getRedirect();
    std::string getEndPoint();
    std::string getDirPage();
    std::vector<std::string> getErrorPage();
    bool getCgiLock();
};

std::vector<std::string> spliteString(std::string str);

#endif
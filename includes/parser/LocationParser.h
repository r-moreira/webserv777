#ifndef LocationParser_hpp
#define LocationParser_hpp
#include <iostream>
#include <vector>

#include "../config/Location.h"
class LocationParser
{
private:
    Location _location;
    void parcerLimitExcept(std::string location);
    void parcerRoot(std::string location);
    void parcerRedirect(std::string location);
    void parcerCgiPath(std::string location);
    void parcerEndPoint(std::string location);
    void parcerDirPage(std::string location);
    void parcerErrorPage(std::string location);
    int contains(std::string delimiter, std::string str);
    void parcerUploadPath(std::string location);
    void parcerMaxBodySize(std::string location);
    void parcerAutoIndex(std::string location);
    void parcerIndex(std::string location);
public:
    LocationParser();
    void addNewAtribute(std::string str);
    Location getLocation();
};

std::vector<std::string> spliteString(std::string str);

#endif
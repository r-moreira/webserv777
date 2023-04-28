#include "../../includes/ConfigParcer/Location.hpp"

Location::Location(std::string location) {
    parcerEndPoint(location);
    std::cout << "lets go my friend" << '\n';
    //parcerLimitExcept(location);
    //parcerRoot(location);
    //parcerReturnPage(location);
    //parcerCgiLock(location);
}

void Location::parcerRoot(std::string location) {
    std::string delimiter = "root ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    root = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
}

void Location::parcerLimitExcept(std::string location) {
    std::string delimiter = "limit_except ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    limitExcept = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
}

void Location::parcerReturnPage(std::string location) {
    std::string delimiter = "return ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    returnPage = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
}

void Location::parcerEndPoint(std::string location) {
    std::string delimiter = "location ";
    std::string endDelimiter = " ";
    int n = delimiter.size();
    endPoint = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
}

void Location::parcerCgiLock(std::string location) {
    std::string delimiter = "cgi_lock";
    if (location.find(delimiter) != location.size()) {
        cgi_lock = 1;
    }
    cgi_lock = 0;
}

bool Location::getCgiLock() {
    return cgi_lock;
}

std::string Location::getEndPoint() {
    return endPoint;
}

std::string Location::getLimitExcept() {
    return limitExcept;
}

std::string Location::getReturnPage() {
    return returnPage;
}

std::string Location::getRoot() {
    return root;
}
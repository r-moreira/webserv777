#include "../../includes/ConfigParcer/Location.hpp"

Location::Location() {}

void Location::addNewAtribute(std::string str) {
    parcerEndPoint(str);
    parcerLimitExcept(str);
    parcerRoot(str);
    parcerReturnPage(str);
    parcerCgiLock(str);
}

void Location::parcerRoot(std::string location) {
    std::string delimiter = "root ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        root = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
    }
}

void Location::parcerLimitExcept(std::string location) {
    std::string delimiter = "limit_except ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        limitExcept = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
    }
}

void Location::parcerReturnPage(std::string location) {
    std::string delimiter = "return ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        returnPage = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
    }
}

void Location::parcerEndPoint(std::string location) {
    std::string delimiter = "location ";
    std::string endDelimiter = " ";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        endPoint = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
    }
}

void Location::parcerCgiLock(std::string location) {
    std::string delimiter = "cgi_lock";
    if (contains(delimiter, location)) {
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

int Location::contains(std::string delimiter, std::string str) {
    return str.find(delimiter) != std::string::npos ? 1 : 0;
}
#include "../../includes/parser/LocationParser.hpp"

LocationParser::LocationParser() {}

void LocationParser::addNewAtribute(std::string str) {
    parcerEndPoint(str);
    parcerLimitExcept(str);
    parcerRoot(str);
    parcerReturnPage(str);
    parcerCgiLock(str);
}

void LocationParser::parcerRoot(std::string location) {
    std::string delimiter = "root ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        root = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
    }
}

void LocationParser::parcerLimitExcept(std::string location) {
    std::string delimiter = "limit_except ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        limitExcept = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
    }
}

void LocationParser::parcerReturnPage(std::string location) {
    std::string delimiter = "return ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        returnPage = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
    }
}

void LocationParser::parcerEndPoint(std::string location) {
    std::string delimiter = "location ";
    std::string endDelimiter = " ";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        endPoint = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
    }
}

void LocationParser::parcerCgiLock(std::string location) {
    std::string delimiter = "cgi_lock";
    if (contains(delimiter, location)) {
        cgi_lock = 1;
    }
    cgi_lock = 0;
}

bool LocationParser::getCgiLock() {
    return cgi_lock;
}

std::string LocationParser::getEndPoint() {
    return endPoint;
}

std::string LocationParser::getLimitExcept() {
    return limitExcept;
}

std::string LocationParser::getReturnPage() {
    return returnPage;
}

std::string LocationParser::getRoot() {
    return root;
}

int LocationParser::contains(std::string delimiter, std::string str) {
    return str.find(delimiter) != std::string::npos ? 1 : 0;
}
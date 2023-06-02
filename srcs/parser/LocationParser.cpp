#include "../../includes/parser/LocationParser.hpp"
#include "../../includes/webserv.h"

LocationParser::LocationParser() {}

void LocationParser::addNewAtribute(std::string str) {
    parcerEndPoint(str);
    parcerLimitExcept(str);
    parcerRoot(str);
    parcerRedirect(str);
    parcerCgiLock(str);
    parcerDirPage(str);
    parcerErrorPage(str);
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
        limitExcept = spliteString(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n));
    }
}

void LocationParser::parcerRedirect(std::string location) {
    std::string delimiter = "redirect ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        redirect = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
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

std::vector<std::string> LocationParser::getLimitExcept() {
    return limitExcept;
}

std::string LocationParser::getRedirect() {
    return redirect;
}

std::string LocationParser::getRoot() {
    return root;
}

int LocationParser::contains(std::string delimiter, std::string str) {
    return str.find(delimiter) != std::string::npos ? 1 : 0;
}

void LocationParser::parcerDirPage(std::string location) {
    std::string delimiter = "directory_page ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        directoryPage = location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n);
    }
}

void LocationParser::parcerErrorPage(std::string location) {
    std::string delimiter = "error_page ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        errorPage = spliteString(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n));
    }
}

std::string LocationParser::getDirPage() {
    return directoryPage;
}

std::vector<std::string> LocationParser::getErrorPage() {
    return errorPage;
}

std::vector<std::string> spliteString(std::string str) {
    std::istringstream iss(str);
    std::vector<std::string> words;

    std::string word;
    while (std::getline(iss, word, ' ')) {
        words.push_back(word);
    }
    return words;
}

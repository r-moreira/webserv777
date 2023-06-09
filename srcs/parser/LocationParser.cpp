#include "../../includes/parser/LocationParser.h"
#include "../../includes/webserv.h"

LocationParser::LocationParser() {}

void LocationParser::addNewAtribute(std::string str) {
    parcerEndPoint(str);
    parcerLimitExcept(str);
    parcerRoot(str);
    parcerRedirect(str);
    parcerDirPage(str);
    parcerErrorPage(str);
    parcerCgiPath(str);
    parcerUploadPath(str);
    parcerMaxBodySize(str);
    parcerAutoIndex(str);
}

void LocationParser::parcerRoot(std::string location) {
    std::string delimiter = "root ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        _location.setRoot(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n));
    }
}

void LocationParser::parcerLimitExcept(std::string location) {
    std::string delimiter = "limit_except ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        _location.setLimitExcept(spliteString(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n)));
    }
}

void LocationParser::parcerRedirect(std::string location) {
    std::string delimiter = "redirect ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        _location.setRedirectLocation(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n));
        _location.setRedirectLock(true);
    }
}

void LocationParser::parcerEndPoint(std::string location) {
    std::string delimiter = "location ";
    std::string endDelimiter = " ";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        _location.setPath(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n));
    }
}

int LocationParser::contains(std::string delimiter, std::string str) {
    return str.find(delimiter) != std::string::npos ? 1 : 0;
}

void LocationParser::parcerDirPage(std::string location) {
    std::string delimiter = "directory_page ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        _location.setRoot(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n));
    }
}

void LocationParser::parcerErrorPage(std::string location) {
    std::string delimiter = "error_page ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        std::vector<std::string> errors = spliteString(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n));
        std::map<int, std::string> errorPages;
        for (size_t i = 0; i < errors.size(); i++) {
            int key = std::atoi(errors[i].c_str());
            i += 1;
            errorPages.insert(std::pair<int, std::string>(key, errors[i]));
        }
        _location.setErrorPages(errorPages);
    }
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

void LocationParser::parcerUploadPath(std::string location) {
    std::string delimiter = "upload ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        _location.setUploadPath(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n));
        _location.setUploadLock(true);
    }
}

void LocationParser::parcerCgiPath(std::string location) {
    std::string delimiter = "cgi_path ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        _location.setCgiPath(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n));
        _location.setCgiLock(true);
    }
}

void LocationParser::parcerMaxBodySize(std::string location) {
    std::string delimiter = "max_body_size ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        _location.setMaxBodySize(std::atoi(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n).c_str()));
    }
}

void LocationParser::parcerAutoIndex(std::string location) {
    std::string delimiter = "auto_index ";
    std::string endDelimiter = "\n";
    int n = delimiter.size();
    if (contains(delimiter, location)) {
        _location.setAutoIndex(location.substr(location.find(delimiter) + n, location.find(endDelimiter) - n) == "on" ? 1 : 0);
    }
}

Location LocationParser::getLocation() {
    return _location;
}
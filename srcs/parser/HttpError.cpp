#include "../../includes/parser/HttpError.hpp"

HttpError::HttpError() {}

void HttpError::_htmlGenerate(std::string imagePath, std::string errorCode) {
    std::string doc = "<!DOCTYPE html><html><head><title>" + errorCode + "</title>";
    std::string style = "<style>.center-image {display: flex;justify-content: center;align-items: center;height: 100vh;}</style>";
    std::string body = "</head><body><div class=\"center-image\"><img src=\"" + imagePath + "\"></div></body></html>";
    html = doc + style + body;
}

std::string HttpError::getError400() {
    _htmlGenerate("./images/400.jpg", "400");
    return html;
}

std::string HttpError::getError403() {
    _htmlGenerate("./images/403.jpg", "403");
    return html;
}

std::string HttpError::getError404() {
    _htmlGenerate("./images/404.jpg", "404");
    return html;
}

std::string HttpError::getError405() {
    _htmlGenerate("./images/405.jpg", "405");
    return html;
}

std::string HttpError::getError413() {
    _htmlGenerate("./images/413.jpg", "413");
    return html;
}

std::string HttpError::getError500() {
    _htmlGenerate("./images/500.jpg", "500");
    return html;
}

std::string HttpError::getError501() {
    _htmlGenerate("./images/501.jpg", "501");
    return html;
}

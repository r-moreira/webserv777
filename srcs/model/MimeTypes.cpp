//
// Created by rmore on 10/05/2023.
//

#include "../../includes/model/MimeTypes.hpp"

MimeTypes::MimeTypes() {
    this->_mime_types.insert(std::pair<std::string, std::string>("aac", "audio/aac"));
    this->_mime_types.insert(std::pair<std::string, std::string>("avi", "video/x-msvideo"));
    this->_mime_types.insert(std::pair<std::string, std::string>("bmp", "image/bmp"));
    this->_mime_types.insert(std::pair<std::string, std::string>("css", "text/css"));
    this->_mime_types.insert(std::pair<std::string, std::string>("gif", "image/gif"));
    this->_mime_types.insert(std::pair<std::string, std::string>("ico", "image/vnd.microsoft.icon"));
    this->_mime_types.insert(std::pair<std::string, std::string>("js", "text/javascript"));
    this->_mime_types.insert(std::pair<std::string, std::string>("json", "application/json"));
    this->_mime_types.insert(std::pair<std::string, std::string>("mp3", "audio/mpeg"));
    this->_mime_types.insert(std::pair<std::string, std::string>("mp4", "video/mp4"));
    this->_mime_types.insert(std::pair<std::string, std::string>("otf", "font/otf"));
    this->_mime_types.insert(std::pair<std::string, std::string>("png", "image/png"));
    this->_mime_types.insert(std::pair<std::string, std::string>("php", "application/x-httpd-php"));
    this->_mime_types.insert(std::pair<std::string, std::string>("rtf", "application/rtf"));
    this->_mime_types.insert(std::pair<std::string, std::string>("svg", "image/svg+xml"));
    this->_mime_types.insert(std::pair<std::string, std::string>("txt", "text/plain"));
    this->_mime_types.insert(std::pair<std::string, std::string>("webm", "video/webm"));
    this->_mime_types.insert(std::pair<std::string, std::string>("webp", "video/webp"));
    this->_mime_types.insert(std::pair<std::string, std::string>("woff", "font/woff"));
    this->_mime_types.insert(std::pair<std::string, std::string>("woff2", "font/woff2"));
    this->_mime_types.insert(std::pair<std::string, std::string>("zip", "application/zip"));
    this->_mime_types.insert(std::pair<std::string, std::string>("html", "text/html"));
    this->_mime_types.insert(std::pair<std::string, std::string>("htm", "text/html"));
    this->_mime_types.insert(std::pair<std::string, std::string>("jpeg", "image/jpeg"));
    this->_mime_types.insert(std::pair<std::string, std::string>("jpg", "image/jpeg"));
}

MimeTypes::~MimeTypes() {}

std::string MimeTypes::get_mime_type(const std::string &extension) {
    try {
        return this->_mime_types.at(extension);
    } catch (std::out_of_range &e) {
        return "text/html";
    }
}

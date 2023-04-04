#include "../../includes/http/Http.hpp"

FT::Http::Http(int socket): Read(socket) {}

FT::Http::~Http() {}

std::string FT::Http::getRequest() {
    return get_text();
}
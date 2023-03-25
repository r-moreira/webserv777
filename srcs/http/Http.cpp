#include "../../includes/http/Http.hpp"

FT::Http::Http(int socket): Reading(socket) {
    request = new HttpRequest(get_text());
}

FT::Http::~Http() {
    delete request;
}

FT::HttpRequest* FT::Http::get_request() {
    return request;
}
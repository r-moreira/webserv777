//
// Created by rmore on 10/05/2023.
//

#include "../../includes/model/Pages.h"

Pages::Pages() {}

Pages::~Pages() {}

std::string Pages::get_default_error_page(event_http_status_enum_t status) {
    std::ostringstream error_page;
    std::string html_tag_init = "<html><body><h1>";
    std::string html_message = "Webserv Error: ";
    long error_status_code = status;
    std::string html_tag_end =  "</h1></body></html>";

    error_page << html_tag_init << html_message << error_status_code << html_tag_end;
    return error_page.str();
}

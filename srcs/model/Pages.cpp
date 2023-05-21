//
// Created by rmore on 10/05/2023.
//

#include "../../includes/model/Pages.h"

Pages::Pages() {}

Pages::~Pages() {}

std::string Pages::get_default_error_page(Event::event_http_status status) {
    std::string error400 = "<!DOCTYPEhtml><html><head><metacharset=\"UTF-8\"><metahttp-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><metaname=\"viewport\"content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\"><title>Server Default 400</title><style>html,body{width:100%;height:100%;margin:0;padding:0;}body{display:flex;align-items:center;justify-content:center;background-color:#424;font-size:14px;}h3{font-size:60px;color:#eee;text-align:center;padding-top:30px;font-weight:normal;}</style></head><body><h3>ERROR 400 - BAD REQUEST</h3></body></html>";
    std::string error403 = "<!DOCTYPEhtml><html><head><metacharset=\"UTF-8\"><metahttp-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><metaname=\"viewport\"content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\"><title>Server Default 403</title><style>html,body{width:100%;height:100%;margin:0;padding:0;}body{display:flex;align-items:center;justify-content:center;background-color:#424;font-size:14px;}h3{font-size:60px;color:#eee;text-align:center;padding-top:30px;font-weight:normal;}</style></head><body><h3>ERROR 403 - FORBIDDEN</h3></body></html>";
    std::string error404 = "<!DOCTYPEhtml><html><head><metacharset=\"UTF-8\"><metahttp-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><metaname=\"viewport\"content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\"><title>Server Default 404</title><style>html,body{width:100%;height:100%;margin:0;padding:0;}body{display:flex;align-items:center;justify-content:center;background-color:#424;font-size:14px;}h3{font-size:60px;color:#eee;text-align:center;padding-top:30px;font-weight:normal;}</style></head><body><h3>ERROR 404 - NOT FOUND</h3></body></html>";
    std::string error405 = "<!DOCTYPEhtml><html><head><metacharset=\"UTF-8\"><metahttp-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><metaname=\"viewport\"content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\"><title>Server Default 405</title><style>html,body{width:100%;height:100%;margin:0;padding:0;}body{display:flex;align-items:center;justify-content:center;background-color:#424;font-size:14px;}h3{font-size:60px;color:#eee;text-align:center;padding-top:30px;font-weight:normal;}</style></head><body><h3>ERROR 405 - METHOD NOT ALLOWED</h3></body></html>";
    std::string error413 = "<!DOCTYPEhtml><html><head><metacharset=\"UTF-8\"><metahttp-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><metaname=\"viewport\"content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\"><title>Server Default 413</title><style>html,body{width:100%;height:100%;margin:0;padding:0;}body{display:flex;align-items:center;justify-content:center;background-color:#424;font-size:14px;}h3{font-size:60px;color:#eee;text-align:center;padding-top:30px;font-weight:normal;}</style></head><body><h3>ERROR 413 - PAYLOAD TOO LARGE</h3></body></html>";
    std::string error418 = "<!DOCTYPEhtml><html><head><metacharset=\"UTF-8\"><metahttp-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><metaname=\"viewport\"content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\"><title>Server Default 418</title><style>html,body{width:100%;height:100%;margin:0;padding:0;}body{display:flex;align-items:center;justify-content:center;background-color:#424;font-size:14px;}h3{font-size:60px;color:#eee;text-align:center;padding-top:30px;font-weight:normal;}</style></head><body><h3>ERROR 418 - I'M A TEAPOT :)</h3></body></html>";
    std::string error500 = "<!DOCTYPEhtml><html><head><metacharset=\"UTF-8\"><metahttp-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><metaname=\"viewport\"content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\"><title>Server Default 500</title><style>html,body{width:100%;height:100%;margin:0;padding:0;}body{display:flex;align-items:center;justify-content:center;background-color:#424;font-size:14px;}h3{font-size:60px;color:#eee;text-align:center;padding-top:30px;font-weight:normal;}</style></head><body><h3>ERROR 500 - INTERNAL SERVER ERROR</h3></body></html>";
    std::string error501 = "<!DOCTYPEhtml><html><head><metacharset=\"UTF-8\"><metahttp-equiv=\"X-UA-Compatible\"content=\"IE=edge\"><metaname=\"viewport\"content=\"width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no\"><title>Server Default 501</title><style>html,body{width:100%;height:100%;margin:0;padding:0;}body{display:flex;align-items:center;justify-content:center;background-color:#424;font-size:14px;}h3{font-size:60px;color:#eee;text-align:center;padding-top:30px;font-weight:normal;}</style></head><body><h3>ERROR 501 - NOT IMPLEMENTED</h3></body></html>";

    switch (status) {
        case Event::BAD_REQUEST: return error400;
        case Event::FORBIDDEN: return error403;
        case Event::NOT_FOUND: return error404;
        case Event::METHOD_NOT_ALLOWED: return error405;
        case Event::PAYLOAD_TOO_LARGE: return error413;
        case Event::INTERNAL_SERVER_ERROR: return error500;
        case Event::NOT_IMPLEMENTED: return error501;
        default: return error418;
    }
}
#include "../../includes/model/AutoIndex.h"

AutoIndex::AutoIndex() {};

std::string AutoIndex::pageGenerator(const std::string& request_root, const std::string& request_uri, int port) {
    std::string page;

    const std::string& path = request_uri;
    DIR *dir = opendir(request_root.c_str());
    if (dir == NULL) {
        std::cout << "Error: could not open this folder!\n";
        return "404";
    }
    page = "<!DOCTYPE html>\n\
        <html>\n\
        <head>\n\
        <title>" + path + "</title>\n\
        </head>\n\
        <body>\n\
        <h1>AUTO INDEX</h1>\n\
        <p>\n";

    for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) {
        dirEntry->d_type == DT_DIR
            ? page += _linkGenerator(request_uri, std::string(dirEntry->d_name) + "/", port)
            : page += _linkGenerator(request_uri, std::string(dirEntry->d_name), port);
    }

    page += "\
          </p>\n\
          </body>\n\
          </html>\n";
    closedir(dir);
    return page;
}

std::string AutoIndex::_linkGenerator(const std::string& pathLink, const std::string& dirEntry, int port) {
    std::stringstream ss;
    ss << "\t\t<p><a href=\"http://localhost" << ":" << port
       << pathLink + (pathLink[pathLink.size() - 1] != '/' ? "/" : "") + dirEntry + "\">" + dirEntry + "</a></p>\n";
    return ss.str();
}
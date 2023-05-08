#include "../../includes/parser/Autoindex.hpp"

Autoindex::Autoindex() {};

std::string Autoindex::pageGenerator(const std::string target, const Server &serve) {
  std::string page;
  //TODO remove the public and put server root
  std::string path = "./public" + target;
  DIR *dir = opendir(path.c_str());
  if (dir == NULL){
    std::cout << "Error: could not open this folder!\n";
    return "404";
  }
  page = "<!DOCTYPE html>\n\
        <html>\n\
        <head>\n\
        <title>" + path + "</title>\n\
        </head>\n\
        <body>\n\
        <h1>INDEX</h1>\n\
        <p>\n";

  for(struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
    page += _linkGenerator(target, std::string(dirEntry->d_name), serve);
  page += "\
          </p>\n\
          </body>\n\
          </html>\n";
  closedir(dir);
  return page;
}

std::string Autoindex::_linkGenerator(const std::string pathLink, std::string dirEntry, const Server &server){
  std::stringstream ss;
  ss << "\t\t<p><a href=\"http://localhost" << ":" << server.getPort()
     << pathLink + (pathLink[pathLink.size() - 1] != '/' ? "/" : "") + dirEntry + "\">" + dirEntry + "</a></p>\n";
  return ss.str();
}

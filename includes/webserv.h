//
// Created by rmore on 15/03/2023.
//

#ifndef WEBSERV_WEBSERV_H
#define WEBSERV_WEBSERV_H

#include <iostream>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include <sys/sendfile.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fstream>
#include <fcntl.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */

#define MAX_EPOLL_EVENTS 64
#define REQUEST_READ_BUFFER_SIZE 1024
#define FILE_READ_CHUNK_SIZE 65536
#define BODY_READ_BUFFER_SIZE 1024
#define CGI_BUFFER_SIZE 1024

#define ITOSTR( x ) dynamic_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x )).str()

#endif //WEBSERV_WEBSERV_H

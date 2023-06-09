//
// Created by rmore on 09/06/2023.
//

#ifndef WEBSERV_LOGGER_H
#define WEBSERV_LOGGER_H

#include "../webserv.h"

class Logger {
    enum Level {
        INFO,
        ERROR,
        WARNING,
        DEBUG,
        TRACE
    };

public:
    static void info(const std::string &message);

    static void error(const std::string &message);

    static void warning(const std::string &message);

    static void debug(const std::string &message);

    static void trace(const std::string &message);

private:
    static void _log(const std::string &message,  Logger::Level level);

    static std::string _date_now();

};


#endif //WEBSERV_LOGGER_H

//
// Created by rmore on 09/06/2023.
//

#include "../../includes/utils/Logger.h"

void Logger::_log(const std::string &message, Logger::Level level) {
    switch (level) {
        case INFO:
            std::cout << "[" << _date_now() << "] [" << GREEN "INFO " << RESET << "] " << message << std::endl;
            break;
        case ERROR:
            std::cout << "[" << _date_now() << "] [" << RED "ERROR" << RESET << "] " << message << std::endl;
            break;
        case WARNING:
            std::cout << "[" << _date_now() << "] [" << YELLOW "WARN " << RESET << "] " << message << std::endl;
            break;
        case DEBUG:
            if (ENABLE_LOGGER_DEBUG) {
                std::cout << "[" << _date_now() << "] [" << MAGENTA "DEBUG" << RESET << "] " << message << std::endl;
            }
            break;
        case TRACE:
            if (ENABLE_LOGGER_TRACE) {
                std::cout << "[" << _date_now() << "] [" << CYAN "TRACE" << RESET << "] " << message << std::endl;
            }
    }

}

void Logger::info(const std::string &message) {
    _log(message, INFO);
}

void Logger::error(const std::string &message) {
    if (DISABLE_BROKEN_PIPE_ERROR_LOG) {
        if (message.find("Broken pipe") != std::string::npos) {
            return;
        }
    }
    _log(message, ERROR);
}

void Logger::warning(const std::string &message) {
    _log(message, WARNING);
}

void Logger::debug(const std::string &message) {
    _log(message, DEBUG);
}


void Logger::trace(const std::string &message) {
    _log(message, TRACE);
}

std::string Logger::_date_now() {
    struct tm * timeinfo;
    char buffer[80];

    time_t rawtime;
    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
    std::string str(buffer);
    return str;
}
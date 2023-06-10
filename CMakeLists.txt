cmake_minimum_required(VERSION 3.22.1)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_compile_options(-Wall -Wextra -Werror -std=c++98)
add_compile_options(-std=c++98)

add_executable(
        webserv srcs/webserv.cpp includes/webserv.hpp
        srcs/network/Socket.cpp includes/network/Socket.hpp
        srcs/config/Server.cpp includes/config/Server.hpp
        srcs/io/Multiplexer.cpp includes/io/Multiplexer.hpp
        srcs/domain/Event.cpp includes/domain/Event.hpp
        srcs/domain/EventHandler.cpp includes/domain/EventHandler.hpp
        srcs/domain/Request.cpp includes/domain/Request.hpp
        srcs/domain/Response.cpp includes/domain/Response.hpp
        srcs/io/File.cpp includes/io/File.hpp
        srcs/parser/ConfigParser.cpp srcs/parser/LocationParser.cpp
        srcs/config/Location.cpp includes/config/Location.hpp
        srcs/domain/ErrorState.cpp includes/domain/ErrorState.hpp
        srcs/io/Read.cpp includes/io/Read.hpp
        srcs/io/Write.cpp includes/io/Write.hpp
        srcs/model/Headers.cpp includes/model/Headers.hpp
        srcs/model/MimeTypes.cpp includes/model/MimeTypes.hpp
        srcs/model/Pages.cpp includes/model/Pages.hpp
        srcs/parser/RequestParser.cpp includes/parser/RequestParser.hpp
        srcs/model/RequestData.cpp includes/model/RequestData.hpp
        srcs/model/AutoIndex.cpp includes/model/AutoIndex.hpp
        srcs/cgi/ExecPython.cpp includes/cgi/ExecPython.hpp
        srcs/utils/Logger.cpp includes/utils/Logger.hpp
        srcs/cgi/Exec.cpp includes/cgi/Exec.hpp
        srcs/cgi/Environment.cpp includes/cgi/Environment.hpp
        srcs/config/AutoIndexOption.cpp includes/config/AutoIndexOption.hpp)

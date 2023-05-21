cmake_minimum_required(VERSION 3.22.1)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

#add_compile_options(-Wall -Wextra -Werror -std=c++98)
add_compile_options(-std=c++98)

add_executable(
        webserv srcs/webserv.cpp includes/webserv.h
        srcs/network/Socket.cpp includes/network/Socket.h
        srcs/config/Server.cpp includes/config/Server.h
        srcs/io/Multiplexer.cpp includes/io/Multiplexer.h
        srcs/domain/Event.cpp includes/domain/Event.h
        srcs/domain/EventHandler.cpp includes/domain/EventHandler.h
        srcs/domain/Request.cpp includes/domain/Request.h
        srcs/domain/Response.cpp includes/domain/Response.h
        srcs/io/File.cpp includes/io/File.h
        srcs/parser/ConfigParser.cpp srcs/parser/LocationParser.cpp
        srcs/config/Location.cpp includes/config/Location.h
        srcs/domain/ErrorState.cpp includes/domain/ErrorState.h
        srcs/io/Read.cpp includes/io/Read.h
        srcs/io/Write.cpp includes/io/Write.h
        srcs/model/Headers.cpp includes/model/Headers.h
        srcs/model/MimeTypes.cpp includes/model/MimeTypes.h
        srcs/model/Pages.cpp includes/model/Pages.h
        srcs/parser/RequestParser.cpp includes/parser/RequestParser.h srcs/model/RequestData.cpp includes/model/RequestData.h srcs/parser/Autoindex.cpp)

cmake_minimum_required(VERSION 3.24)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

#add_compile_options(-Wall -Wextra -Werror -std=c++98)
add_compile_options(-std=c++98)

add_executable(
        webserv srcs/webserv.cpp includes/webserv.h
        srcs/config/Configuration.cpp includes/config/Configuration.h
        srcs/network/Network.cpp includes/network/Network.h
        srcs/network/Socket.cpp includes/network/Socket.h
        srcs/network/Location.cpp includes/network/Location.h
        srcs/network/Server.cpp includes/network/Server.h
        srcs/io/Multiplexer.cpp includes/io/Multiplexer.h
        srcs/domain/Request.cpp includes/domain/Request.h
        srcs/parser/HttpRequestParser.cpp includes/parser/HttpRequestParser.h srcs/parser/UrlParser.cpp includes/parser/UrlParser.h srcs/domain/Response.cpp includes/domain/Response.h srcs/parser/HttpResponseParser.cpp includes/parser/HttpResponseParser.h srcs/domain/Event.cpp includes/domain/Event.h)

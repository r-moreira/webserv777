cmake_minimum_required(VERSION 3.22.1)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

#add_compile_options(-Wall -Wextra -Werror -std=c++98)
add_compile_options(-std=c++98)

add_executable(
        webserv srcs/webserv.cpp includes/webserv.h
        srcs/network/Socket.cpp includes/network/Socket.h
        srcs/network/Server.cpp includes/network/Server.h
        srcs/io/Multiplexer.cpp includes/io/Multiplexer.h
        includes/parser/RequestInfo.h
        includes/parser/HttpRequestParser.h
        includes/parser/UrlParser.h
        includes/parser/ResponseInfo.h
        includes/parser/HttpResponseParser.h
        srcs/domain/Event.cpp includes/domain/Event.h
        srcs/domain/EventHandler.cpp includes/domain/EventHandler.h
        srcs/domain/Request.cpp includes/domain/Request.h
        srcs/domain/Response.cpp includes/domain/Response.h
        srcs/domain/File.cpp includes/domain/File.h
        srcs/parser/ConfigParser.cpp srcs/parser/LocationParser.cpp
        srcs/network/Location.cpp includes/network/Location.h srcs/domain/EventStateHelper.cpp includes/domain/EventStateHelper.h srcs/io/Read.cpp includes/io/Read.h)

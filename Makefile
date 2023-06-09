NAME	 			= webserv
CC 					= g++
CFLAGS 				=  -g -std=c++98 -Wall -Wextra -Werror
OBJS_DIR			= objs
OBJS				= $(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))
SRCS_DIR			= srcs
CONFIG_DIR			= ${SRCS_DIR}/config
DOMAIN_DIR			= ${SRCS_DIR}/domain
IO_DIR				= ${SRCS_DIR}/io
MODEL_DIR			= ${SRCS_DIR}/model
NETWORK_DIR			= ${SRCS_DIR}/network
PARSER_DIR			= ${SRCS_DIR}/parser
CGI_DIR				= ${SRCS_DIR}/cgi
UTILS_DIR			= ${SRCS_DIR}/utils

SRCS				= ${SRCS_DIR}/webserv.cpp\
					  ${CONFIG_DIR}/Location.cpp\
					  ${CONFIG_DIR}/Server.cpp\
					  ${CONFIG_DIR}/AutoIndexOption.cpp\
					  ${DOMAIN_DIR}/ErrorState.cpp\
					  ${DOMAIN_DIR}/Event.cpp\
					  ${DOMAIN_DIR}/EventHandler.cpp\
					  ${DOMAIN_DIR}/Request.cpp\
					  ${DOMAIN_DIR}/Response.cpp\
					  ${IO_DIR}/File.cpp\
					  ${IO_DIR}/Multiplexer.cpp\
					  ${IO_DIR}/Read.cpp\
					  ${IO_DIR}/Write.cpp\
					  ${MODEL_DIR}/Headers.cpp\
					  ${MODEL_DIR}/MimeTypes.cpp\
					  ${MODEL_DIR}/Pages.cpp\
					  ${MODEL_DIR}/RequestData.cpp\
					  ${MODEL_DIR}/AutoIndex.cpp\
					  ${NETWORK_DIR}/Socket.cpp\
					  ${PARSER_DIR}/RequestParser.cpp\
					  ${PARSER_DIR}/ConfigParser.cpp\
					  ${PARSER_DIR}/LocationParser.cpp\
					  ${CGI_DIR}/Environment.cpp\
					  ${CGI_DIR}/Exec.cpp\
					  ${CGI_DIR}/ExecPython.cpp\
					  ${UTILS_DIR}/Logger.cpp

all:				$(NAME)

$(NAME):			$(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o:	$(SRCS_DIR)/%.cpp
	$(CC) $(CFLAGS) -I/includes/ -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:				clean
	$(RM) $(NAME)

re:					fclean all

.PHONY:	all clen fclean re

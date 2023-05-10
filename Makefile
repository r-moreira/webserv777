NAME	 			= webserv
CC 					= c++
CFLAGS 				=  -g -std=c++98 #-Wall -Wextra -Werror
OBJS_DIR			= objs
OBJS				= $(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))
SRCS_DIR			= srcs
NETWORK_DIR			= ${SRCS_DIR}/network
PARSER_DIR			= ${SRCS_DIR}/parser
DOMAIN_DIR			= ${SRCS_DIR}/domain
MODEL_DIR			= ${SRCS_DIR}/model
IO_DIR				= ${SRCS_DIR}/io

SRCS				= ${SRCS_DIR}/webserv.cpp\
					  ${DOMAIN_DIR}/Event.cpp\
					  ${DOMAIN_DIR}/EventHandler.cpp\
					  ${DOMAIN_DIR}/File.cpp\
					  ${DOMAIN_DIR}/Request.cpp\
					  ${DOMAIN_DIR}/Response.cpp\
					  ${DOMAIN_DIR}/EventStateHelper.cpp\
					  ${IO_DIR}/Multiplexer.cpp\
					  ${NETWORK_DIR}/Server.cpp\
					  ${NETWORK_DIR}/Socket.cpp\
					  ${NETWORK_DIR}/Location.cpp\
					  ${PARSER_DIR}/ConfigParser.cpp\
					  ${PARSER_DIR}/LocationParser.cpp\

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

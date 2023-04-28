NAME	 			= webserv
CC 					= c++
CFLAGS 				=  -g -std=c++98 #-Wall -Wextra -Werror
OBJS_DIR			= objs
OBJS				= $(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))
SRCS_DIR			= srcs
CONF_DIR			= ${SRCS_DIR}/config
NETWORK_DIR			= ${SRCS_DIR}/network
PARSER_DIR			= ${SRCS_DIR}/parser
DOMAIN_DIR			= ${SRCS_DIR}/domain
IO_DIR				= ${SRCS_DIR}/io

SRCS				= ${SRCS_DIR}/webserv.cpp\
					  ${CONF_DIR}/Configuration.cpp\
					  ${DOMAIN_DIR}/Event.cpp\
					  ${DOMAIN_DIR}/EventHandler.cpp\
					  ${DOMAIN_DIR}/File.cpp\
					  ${DOMAIN_DIR}/Request.cpp\
					  ${DOMAIN_DIR}/Response.cpp\
					  ${IO_DIR}/Multiplexer.cpp\
					  ${NETWORK_DIR}/Server.cpp\
					  ${NETWORK_DIR}/ServerBuilder.cpp\
					  ${NETWORK_DIR}/Socket.cpp\
					  ${PARSER_DIR}/ConfigParcer.cpp\
					  ${PARSER_DIR}/Location.cpp\

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

NAME	 			= webserv
CC 					= c++
CFLAGS 				=  
OBJS_DIR			= objs
SRCS_DIR			= srcs
SOCK_DIR = Sockets
SERV_DIR = Server
CONF_DIR			= ${SRCS_DIR}/config
NETWORK_DIR			= ${SRCS_DIR}/network
PARSER_DIR			= ${SRCS_DIR}/parser
DOMAIN_DIR			= ${SRCS_DIR}/domain
IO_DIR				= ${SRCS_DIR}/io

SRCS = $(addprefix ./srcs/, \
          	main.cpp \
          	${SOCK_DIR}/BindingSocket.cpp \
          	${SOCK_DIR}/Connecting.cpp \
			${SOCK_DIR}/ListeningSocket.cpp \
			${SOCK_DIR}/Socket.cpp \
			${SERV_DIR}/Server.cpp \
			${SERV_DIR}/ServerTemplate.cpp)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(<:%.c=%.o)

OBJS = $(SRCS:%.c=%.o)

all:	$(NAME)

$(NAME):			$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean:				clean
	$(RM) $(NAME)

re:					fclean all

.PHONY:	all clen fclean re

NAME	= webserv

SRCS	= ${wildcard *.cpp} ${wildcard */*.cpp} ${wildcard */*/*.cpp}
OBJS	= ${SRCS:.cpp=.o}
CC		= c++ #-fsanitize=address -ggdb3 -g
#CFLAGS	= -Wall -Wextra -Werror -std=c++98
RM		= rm -rf

all: ${NAME}

.cpp.o:
	@$(CC) $(CFLAGS) -c $< -o $(<:.cpp=.o)

${NAME}: ${OBJS}
	@${CC} ${OBJS} ${CFLAGS} -o ${NAME}

clean:
	@${RM} ${OBJS}

fclean: clean
	@${RM} ${NAME}

re: fclean all

.PHONY: clean fclean re all

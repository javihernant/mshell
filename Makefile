NAME=minishell
CC=gcc
CFLAGS=-Wall -Wextra -Werror
IFLAGS=-I./include
SRC=tests/lexer_test.c src/lexer.c src/strings/str1.c
OBJ=$(SRC:.c=.o)
COMPILED_OBJS=$(shell find . -name '*.o')
DBG=-g

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJ) -o $(NAME) $(LIBS) $(DBG)

%.o:%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@ $(DBG)

fclean: clean
	rm -rf $(NAME)

clean:
	rm -rf $(COMPILED_OBJS)

re: fclean all
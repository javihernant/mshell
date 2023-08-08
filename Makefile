NAME=minishell
CC=gcc
CFLAGS=-Wall -Wextra -Werror
IFLAGS=-I./include
TESTS=tests/lexer_test.c tests/parser_test.c
SRC=src/lexer.c src/strings/str1.c src/lists/lists1.c src/parser.c src/errors.c
OBJ=$(SRC:.c=.o) $(TESTS:.c=.o)
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
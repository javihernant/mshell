NAME=minishell
CC=gcc
CFLAGS=-Wall -Wextra -Werror
IFLAGS=-I. -I./include
TESTS=tests/lexer_test.c tests/parser_test.c tests/visit_expr_test.c tests/test_globs.c munit/munit.c
SRC=src/strings/str1.c src/lists/lists1.c src/errors.c src/globs/globs.c src/globs/look_dir.c src/exec/args_process.c src/exec/expansion.c src/exec/find_binary.c src/exec/redir.c src/exec/visit_expr.c src/exec/visit_expr2.c src/interpret/lexer1.c src/interpret/lexer2.c src/interpret/lexer3.c src/interpret/lexer4.c src/interpret/parser_aux.c src/interpret/parser.c src/interpret/preprocess.c
OBJ=$(SRC:.c=.o) $(TESTS:.c=.o)
COMPILED_OBJS=$(shell find . -name '*.o')
DBG=-g
LIBS=-lreadline

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJ) -o $(NAME) $(LIBS) $(DBG)

%.o:%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@ $(DBG) $(LIBS)

fclean: clean
	rm -rf $(NAME)

clean:
	rm -rf $(COMPILED_OBJS)

re: fclean all
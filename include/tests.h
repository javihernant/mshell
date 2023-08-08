#ifndef TESTS_H
# define TESTS_H

#include <stdio.h>
#include "lists_fts.h"
#include "exprs.h"

int		skip_char(char *line, int i, int j);
char	*parse_arg(char *line, int *idx);
t_list	*parse_args(char *line, int *idx);
t_list	*get_tokens(char *line);
t_expr	*gen_expr(char *line);
void	print_lst(t_list *lst);

void	test_parse_args_aux(char *line);
void	test_get_tokens_aux(char *line);
void	print_token(t_token *token);

#endif
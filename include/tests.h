#ifndef TESTS_H
# define TESTS_H

#include <stdio.h>
#include "lists_fts.h"
#include "exprs.h"

int		skip_char(char *line, int i, int j);
t_arg	*parse_arg(char *line, int *idx);
t_list	*parse_cmds(char *line, int *idx);
t_list	*parse_cmd(char *line, int *idx);
t_list	*get_tokens(char *line);
t_expr	*gen_expr(char *line);
void	print_cmds(t_list *cmds);
void	test_parse_args_aux(char *line);
void	test_get_tokens_aux(char *line);
void	print_token(t_token *token);
int		exec_cmd(t_list *args);
int	check_pattern(char *pat, char *str);


#endif
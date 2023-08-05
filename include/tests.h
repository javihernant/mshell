#ifndef TESTS_H
# define TESTS_H

#include <stdio.h>
#include "lists_fts.h"

int		skip_char(char *line, int i, int j);
char	*parse_arg(char *line, int *idx);
t_list	*parse_args(char *line, int *idx);

#endif
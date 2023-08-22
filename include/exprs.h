#ifndef EXPRS_H
# define EXPRS_H

#include "lists_fts.h"

# define TKN_CMDS 0
# define TKN_AND 1
# define TKN_OR 2
# define TKN_LPAR 3
# define TKN_RPAR 4

typedef struct s_token
{
	int		type;
	t_list	*cmds;
}	t_token;

# define EXPR_CMDS 0
# define EXPR_AND 1
# define EXPR_OR 2
# define EXPR_PAR 3

typedef struct s_expr
{
	int				type;
	struct s_expr	*expr_a;
	struct s_expr	*expr_b;
	t_list			*cmds;
}	t_expr;

// # define CMD_DFLT 0
// # define CMD_PIPE 1
// typedef struct s_cmd
// {
// 	int				type;
// 	t_list			*args;
// }	t_cmd;

/*
	List of commands
		List of args
*/

# define ARG_DFLT 0
# define ARG_DFLT_SGL 1
# define ARG_DFLT_DBL 2
# define ARG_IN 3
# define ARG_HDOC 4
# define ARG_OUT 5
# define ARG_OUTAPND 6
# define ARG_PIPE 7

typedef struct s_arg
{
	int				type;
	char			*arg;
}	t_arg;

void	free_arg(t_arg *arg);
void	free_cmd(t_list *args);
void	free_cmds(t_list *cmds);
void	free_expr(t_expr *expr);

#endif
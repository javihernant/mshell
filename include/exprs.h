#ifndef EXPRS_H
# define EXPRS_H

#include "lists_fts.h"

# define TKN_ARGS 0
# define TKN_AND 1
# define TKN_OR 2
# define TKN_LPAR 3
# define TKN_RPAR 4

typedef struct s_token
{
	int		type;
	t_list	*args;
}	t_token;

# define EXPR_ARGS 0
# define EXPR_AND 1
# define EXPR_OR 2
# define EXPR_PAR 3

typedef struct s_expr
{
	int				type;
	struct s_expr	*expr_a;
	struct s_expr	*expr_b;
	t_list			*args;
}	t_expr;

#endif
#ifndef TOKENS_H
# define TOKENS_H

# define TKN_ARGS 0
# define TKN_AND 1
# define TKN_OR 2
# define TKN_LPAR 3
# define TKN_RPAR 4


#include "lists_fts.h"

typedef struct s_token
{
	int		type;
	t_list	args;
}	t_token;
#endif
#include "strings_fts.h"
#include "lists_fts.h"
#include "exprs.h"
#include <stdlib.h>

int	skip_char(char *line, int i, int j)
{
	if (j > i && line[j - 1] == '\\')
	{
		if (j - 1 > i && line[j - 2] == '\\')
		{
			if (j - 2 > i && line[j - 3] == '\\')
				return (1);
			else
				return (0);
		}
		else
			return (1);
	}
	else
		return (0);
}

int	is_concat_op(char *line, int idx)
{
	if (line[idx] != '\0' && line[idx + 1] != '\0')
	{
		if (line[idx] == '|' && line[idx + 1] == '|')
			return (1);
		if (line[idx] == '&' && line[idx + 1] == '&')
			return (1);
	}
	return (0);
}

int	is_end_of_arg(char first_c, char *line, int i, int j)
{
	if (first_c != 0 && line[j] == first_c)
	{
		if (!skip_char(line, i, j))
			return (1);
	}
	else if (first_c == 0 && (is_space(line[j]) || is_concat_op(line, j)))
		return (1);
	return (0);
}

char	*parse_arg(char *line, int *idx)
{
	int		i;
	int		j;
	char	first_c;

	i = *idx;
	j = i;
	first_c = 0;
	if (line[i] == '\'' || line[i] == '"')
	{
		first_c = line[i];
		i++;
		j++;
	}
	while (line[j] != '\0')
	{
		if (is_end_of_arg(first_c, line, i, j))
			break ;
		j++ ;
	}
	if (first_c == 0)
		*idx = j;
	else
		*idx = j + 1;
	return (str_slice(line, i, j));
}

int is_par(char c)
{
	return (c == '(' || c == ')');
}

t_list	*parse_args(char *line, int *idx)
{
	int		i;
	t_list	*lst;

	lst = 0;
	i = *idx;
	while (1)
	{
		while (is_space(line[i]))
			i++;
		if (line[i] == '\0' || is_concat_op(line, i) || is_par(line[i]))
			break ;
		lstadd_back(&lst, parse_arg(line, &i));
	}
	*idx = i;
	return lst;
}

t_token *mk_tkn(int type, t_list *lst)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->type = type;
	token->args = 0;
	if (type == TKN_ARGS)
		token->args = lst;
	return (token);
}

t_token *gen_par_tkn(char *line, int *idx)
{
	t_token	*token;

	if (line[*idx] == '(')
	{
		token = mk_tkn(TKN_LPAR, 0);
		(*idx)++;
	}
	else
	{
		token = mk_tkn(TKN_RPAR, 0);
		(*idx)++;
	}
	return (token);
}

t_token *gen_op_tkn(char *line, int *idx)
{
	t_token	*token;

	if (line[*idx] == '|')
	{
		token = mk_tkn(TKN_OR, 0);
		*idx += 2;
	}
	else
	{
		token = mk_tkn(TKN_AND, 0);
		*idx += 2;
	}
	return (token);
}

t_token	*produce_token(char *line, int *idx)
{
	t_token	*token;

	if (line[*idx] == '(' || line[*idx] == ')')
		token = gen_par_tkn(line, idx);
	else if (is_concat_op(line, *idx))
		token = gen_op_tkn(line, idx);
	else
		token = mk_tkn(TKN_ARGS, parse_args(line, idx));
	return (token);
}

t_list	*get_tokens(char *line)
{
	int		i;
	t_list	*tokens;

	tokens = 0;
	i = 0;
	while (1)
	{
		while (is_space(line[i]))
			i++;
		if (line[i] == '\0')
			break ;
		lstadd_back(&tokens, produce_token(line, &i));
	}
	return (tokens);
}
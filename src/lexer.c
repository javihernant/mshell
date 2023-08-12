#include "errors.h"
#include "strings_fts.h"
#include "lists_fts.h"
#include "exprs.h"
#include "tests.h"
#include <stdlib.h>

t_arg	*mk_arg(int type, char *str);
int	arg_type(char *line, int idx);

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
	else if (first_c == 0 &&
		(is_space(line[j]) || is_concat_op(line, j)
			|| arg_type(line, i) != ARG_DFLT))
		return (1);
	return (0);
}

char	*parse_str_arg(char *line, int *idx)
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

t_arg *parse_arg(char *line, int *idx)
{
	char	first_c;
	char	*str_arg;
	int		arg_type;

	first_c = 0;
	if (line[*idx] == '\'' || line[*idx] == '"')
		first_c = line[*idx];
	if (first_c == '"')
		arg_type = ARG_DFLT_DBL;
	else if (first_c == '\'')
		arg_type = ARG_DFLT_SGL;
	else
		arg_type = ARG_DFLT;
	str_arg = parse_str_arg(line, idx);
	return (mk_arg(arg_type, str_arg));
}

int is_par(char c)
{
	return (c == '(' || c == ')');
}

int	arg_type(char *line, int idx)
{
	if (line[idx] == '|')
		return (ARG_PIPE);
	else if (line[idx] == '<')
	{
		if (line[idx] != '\0' && line[idx + 1] == '<')
			return (ARG_HDOC);
		else
			return (ARG_IN);
	}
	else if (line[idx] == '>')
	{
		if (line[idx] != '\0' && line[idx + 1] == '>')
			return (ARG_OUTAPND);
		else
			return (ARG_OUT);
	}
	else
		return (ARG_DFLT);
}

t_arg	*mk_arg(int type, char *str)
{
	t_arg	*arg;

	arg = malloc(sizeof(t_arg));
	arg->type = type;
	arg->arg = str;
	return (arg);
}

t_arg	*parse_other_arg(char *line, int *idx)
{
	int		i;
	int		type;
	t_arg	*arg;

	i = *idx;
	type = arg_type(line, i);
	if (type == ARG_HDOC || type == ARG_OUTAPND)
		i += 2;
	else
		i++;
	while (is_space(line[i]))
		i++;
	arg = mk_arg(type, parse_str_arg(line, &i));
	*idx = i;
	return (arg);
}

t_list	*parse_cmd(char *line, int *idx)
{
	t_list	*args;
	int		i;

	args = 0;
	i = *idx;
	while (1)
	{
		while (is_space(line[i]))
			i++;
		if (line[i] == '\0' || is_concat_op(line, i)
			|| is_par(line[i]) || arg_type(line, i) == ARG_PIPE)
			break ;
		if (arg_type(line, i) == ARG_DFLT)
			// lstadd_back(&args, mk_arg(ARG_DFLT, parse_str_arg(line, &i)));
			lstadd_back(&args, parse_arg(line, &i));
		else
			lstadd_back(&args, parse_other_arg(line, &i));
	}
	*idx = i;
	return (args);
}

t_list	*parse_cmds(char *line, int *idx)
{
	t_list	*cmds;
	int		i;

	cmds = 0;
	i = *idx;
	while (1)
	{
		while (is_space(line[i]))
			i++;
		if (line[i] == '\0' || is_concat_op(line, i) || is_par(line[i]))
			break ;
		else if (arg_type(line, i) == ARG_PIPE)
		{
			i++;
			continue ;
		}
		else
			lstadd_back(&cmds, parse_cmd(line, &i));
	}
	*idx = i;
	return (cmds);
}


// t_list	*parse_args(char *line, int *idx)
// {
// 	int		i;
// 	t_list	*lst;

// 	lst = 0;
// 	i = *idx;
// 	while (1)
// 	{
// 		while (is_space(line[i]))
// 			i++;
// 		if (line[i] == '\0' || is_concat_op(line, i) || is_par(line[i]))
// 			break ;
// 		else if(arg_type(line, i) != ARG_LIST)
// 		else
// 			lstadd_back(&lst, parse_arg(line, &i));

// 	}
// 	*idx = i;
// 	return (lst);
// }

t_token *mk_tkn(int type, t_list *lst)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->type = type;
	token->cmds = 0;
	if (type == TKN_CMDS)
		token->cmds = lst;
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
		token = mk_tkn(TKN_CMDS, parse_cmds(line, idx));
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
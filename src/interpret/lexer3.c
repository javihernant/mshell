#include "interpret.h"

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
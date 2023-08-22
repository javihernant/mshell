#include "interpret.h"

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
			lstadd_back(&args, parse_arg(line, &i));
		else
			lstadd_back(&args, parse_other_arg(line, &i));
	}
	*idx = i;
	return (args);
}
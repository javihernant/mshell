#include "interpret.h"

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
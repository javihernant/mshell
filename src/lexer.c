//TODO: char** lexer()
//TODO: char* parse_lexeme()
//TODO: skip_single_quote
//TODO: skip_double_quote
#include "strings_fts.h"
#include "lists_fts.h"

/*
cmd arg1 "skip && || & ' \"" arg3
*/
// t_list	*parse_lexemes() {
// 	//TODO
// }

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

// t_list	*get_tokens(char *line, int *idx)
// {
// 	int		i;
// 	t_list	*lexeme;
// 	char	*arg;

// 	i = *idx;
// 	while (line[i] != '\0')
// 	{
// 		if (is_end_lexeme(line[i]))
// 			break ;
// 		else if (!is_space(line[i]))
// 		{
// 			arg = parse_arg(line, &i);
// 			lstadd_back(&lexeme, (void *)arg);
// 		}
// 		else 
// 			i++;
// 	}
// 	*idx = i;
// 	return (lexeme);
// }
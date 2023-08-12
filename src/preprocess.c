#include <stdio.h>
#include <stdlib.h>
#include "lists_fts.h"
#include "strings_fts.h"

int	skip_char(char *line, int i, int j);

//TEST
char *parse_env_var(char *line, int *idx)
{
	int	i;
	int j;

	i = *idx + 1;
	j = i;
	while (line[j] != '\0' && !is_space(line[j]))
		j++;
	*idx = j;
	if (i == j)
		return (0);
	return (str_slice(line, i, j));
}

//TEST
char *get_env_var(char *line, int *idx)
{
	char	*var;
	char	*val;

	var = parse_env_var(line, idx);
	val = 0;
	if (var != 0)
	{
		val = getenv(var);
		free(var);
	}
	return (val);
}

//TEST
void	skip_single_qt(char *line, int *idx)
{
	int	i;

	i = *idx + 1;
	while (line[i] != '\0')
	{
		if (line[i] == '\'' && !skip_char(line, 0, i))
		{
			i++;
			break ;
		}
		i++;
	}
	*idx = i;
}

//TEST
char *expand_envs(char *line)
{
	int		i;
	int		j;
	t_list	*strs;

	i = 0;
	j = 0;
	strs = 0;
	while (1)
	{
		if (line[j] == '\0')
			break ;
		else if (line[j] == '\'')
			skip_single_qt(line, &j);
		else if (line[j] == '$')
		{
			lstadd_back(&strs, str_slice(line, i, j));
			lstadd_back(&strs, get_env_var(line, &j));
			i = j;
		}
		else
			j++;
	}
	if (j > i)
		lstadd_back(&strs, str_slice(line, i, j));
	return (joinstrs(strs));
}

int main()
{
	char *line;

	line = expand_envs("cmd1 \"$HELO hello\" '$HELO' $HELO another arg");
	printf("%s\n", line);
}
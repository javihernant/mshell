#include "interpret.h"

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
#include <stdlib.h>

//Create string from i to j exclusive
char	*str_slice(char *str, int i, int j)
{
	char	*slice;
	int		idx;

	slice = malloc((j - i) + 1);
	idx = 0;
	while (i < j)
	{
		slice[idx++] = str[i];
		i++;
	}
	slice[idx] = '\0';
	return (slice);
}

int	is_space(char c)
{
	int	b;

	b = (c == ' ' || c == '\t' || c == '\n');
	return (b);
}
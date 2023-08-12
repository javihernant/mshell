#include "lists_fts.h"
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

int	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int		ft_strsequal(char *sa, char *sb)
{
	while (*sa != '\0' && *sb != '\0')
	{
		if (*sa != *sb)
			return (0);
		sa++;
		sb++;
	}
	if (*sa != 0 || *sb != '\0')
		return (0);
	return (1);
}

//TODO: TEST
int	count_chars(t_list	*strs)
{
	int	i;

	i = 0;
	while (strs != 0)
	{
		if (strs->content != 0)
			i += ft_strlen(strs->content);
		strs = strs->next;
	}
	return (i);
}

//TODO:TEST
int	ft_strcpy(char *dst, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (i);
}

//TODO: TEST
char	*joinstrs(t_list *strs)
{
	int		len;
	char	*joined;
	t_list	*tmp;
	int		i;

	i = 0;
	len = count_chars(strs);
	joined = malloc(len + 1);
	while (strs != 0)
	{
		if (strs->content != 0)
			i += ft_strcpy(&joined[i], strs->content);
		tmp = strs->next;
		free(strs->content);
		free(strs);
		strs = tmp;
	}
	joined[i] = '\0';
	return (joined);
}

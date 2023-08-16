#include "lists_fts.h"
#include <stdio.h>
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

int	bytes_join(t_list *strs, char *sep)
{
	int	chars_cnt;
	int	words_cnt;
	int	sep_len;

	chars_cnt = count_chars(strs);
	words_cnt = lstlen(strs);
	if (words_cnt == 0)
		return (1);
	sep_len = 0;
	if (sep != 0)
		sep_len = ft_strlen(sep);
	return (chars_cnt + (words_cnt - 1) * sep_len + 1);
}

char	*joinstrs(t_list *strs, char *sep)
{
	char	*joined;
	t_list	*tmp;
	int		i;

	i = 0;

	joined = malloc(bytes_join(strs, sep));
	while (strs != 0)
	{
		if (strs->content != 0)
			i += ft_strcpy(&joined[i], strs->content);
		if (sep != 0 && strs->next != 0)
			i += ft_strcpy(&joined[i], sep);
		tmp = strs->next;
		free(strs->content);
		free(strs);
		strs = tmp;
	}
	joined[i] = '\0';
	return (joined);
}

char	*ft_strdup(char *str)
{
	int		len;
	char	*dup;

	len = ft_strlen(str);
	dup = malloc(len + 1);
	ft_strcpy(dup, str);
	return (dup);
}

int	ends_with(char *end, char *str)
{
	int	i;
	int	j;

	i = ft_strlen(str);
	j = ft_strlen(end);

	if (j > i)
		return (0);
	while (end[j - 1] != '\0' && i > 0)
	{
		if (str[i - 1] != end[j - 1])
		{
			return (0);
		}
		j--;
		i--;
	}
	return (1);
}

// int main()
// {
// 	int rc = ends_with("", "hola");

// 	printf("%d", rc);
// }
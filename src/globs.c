#include "lists_fts.h"
#include "strings_fts.h"
#include <stdio.h>
#include <stdlib.h>

t_list	*nxt_patpiece(t_list *patt_pieces)
{
	t_list	*next;

	next = 0;
	if (patt_pieces != 0)
	{
		next = patt_pieces->next;
		free(patt_pieces->content);
		free(patt_pieces);
	}
	return (next);
}

//Pattern has to be a trimmed word
t_list	*slice_pattern(char *pat)
{
	int		i;
	int		j;
	t_list	*pieces;

	i = 0;
	j = i;
	pieces = 0;
	while (pat[j] != '\0')
	{
		if (pat[j] == '*')
		{
			if (j > i)
				lstadd_back(&pieces, str_slice(pat, i, j));
			lstadd_back(&pieces, ft_strdup("*"));
			i = j + 1;
			j = i;
		}
		else
		{
			j++;
		}
	}
	if (j > i)
		lstadd_back(&pieces, str_slice(pat, i, j));
	return (pieces);
}

int	piece_matches(char *pat_piece, char *str, int *idx)
{
	int	i;
	int	j;

	i = *idx;
	j = 0;
	while (pat_piece[j] != '\0' && str[i] != '\0')
	{
		if (pat_piece[j] != str[i])
			break ;
		j++;
		i++;
	}
	if (pat_piece[j] == '\0')
	{
		*idx = i;
		return (1);
	}
	return (0);
}

int check_pat_star(t_list **pat_pieces, char *str, int *idx)
{
	int	match;

	while (*pat_pieces != 0 && ft_strsequal((*pat_pieces)->content, "*"))
		*pat_pieces = nxt_patpiece((*pat_pieces));
	if (*pat_pieces == 0)
		return (1);
	if ((*pat_pieces)->next == 0)
		return (ends_with((*pat_pieces)->content, &str[*idx]));
	else
	{
		while (str[*idx] != '\0')
		{
			match = piece_matches((*pat_pieces)->content, str, idx);
			if (match)
			{
				*pat_pieces = nxt_patpiece(*pat_pieces);
				return (-1);
			}
			(*idx)++;
		}
	}
	return (-1);
}

int	check_pattern_aux(t_list **pat_pieces, char *str, int *idx)
{
	if (ft_strsequal((*pat_pieces)->content, "*"))
	{
		return check_pat_star(pat_pieces, str, idx);
	}
	else
	{
		if (piece_matches((*pat_pieces)->content, str, idx))
		{
			*pat_pieces = nxt_patpiece(*pat_pieces);
			return (-1);
		}
		else
			return (0);
	}
	return (-1);
}

int	check_pattern(char *pat, char *str)
{
	t_list	*pat_pieces;
	int		i;
	int		rc;

	i = 0;
	pat_pieces = slice_pattern(pat);
	while (pat_pieces != 0 && str[i] != '\0')
	{
		rc = check_pattern_aux(&pat_pieces, str, &i);
		if (rc != -1)
		{
			clean_str_ls(pat_pieces);
			return (rc);
		}
	}
	while (pat_pieces != 0 && ft_strsequal(pat_pieces->content, "*"))
		pat_pieces = nxt_patpiece(pat_pieces);
	if (pat_pieces == 0 && str[i] == '\0')
		return (1);
	return (0);
}

// int main()
// {
// 	int rc = check_pattern("*lo*", "lon");
// 	printf("%d",rc);
// }

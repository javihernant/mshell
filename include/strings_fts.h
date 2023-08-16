#ifndef STRING_FTS_H
# define STRING_FTS_H

#include "lists_fts.h"

char	*str_slice(char *str, int i, int j);
int		is_space(char c);
int		ft_strlen(char *s);
int		ft_strsequal(char *sa, char *sb);
char	*joinstrs(t_list *strs, char *sep);
char	*ft_strdup(char *str);
int		ends_with(char *end, char *str);
char	*ft_itoa(int n);

#endif
#include "exec.h"

t_list	*ls_paths_aux(char *path_var)
{
	int		i;
	int		j;
	t_list	*paths;

	i = 0;
	j = 0;
	paths = 0;
	while (path_var[j] != '\0')
	{
		if (path_var[j] == ':')
		{
			lstadd_back(&paths, str_slice(path_var, i, j));
			j++;
			i = j;
		}
		else
			j++;
	}
	if (j > i)
		lstadd_back(&paths, str_slice(path_var, i, j));
	return (paths);
}

t_list	*ls_paths()
{
	char	*path_var;

	path_var = getenv("PATH");
	if (path_var == 0)
		return (0);

	return (ls_paths_aux(path_var));
}

t_list	*ls_bin_paths(char *arg)
{
	t_list	*paths;
	t_list	*tmp;
	char	*slash_bin;

	slash_bin = str_merge(ft_strdup("/"), ft_strdup(arg));
	paths = ls_paths();
	tmp = paths;
	while (tmp != 0)
	{
		tmp->content = str_merge(tmp->content, ft_strdup(slash_bin));
		tmp = tmp->next;
	}
	free(slash_bin);
	return (paths);
}

void	find_binary(char **bin_arg)
{
	char	*arg;
	t_list	*bin_paths;
	t_list	*tmp;

	arg = *bin_arg;
	if (access(arg, X_OK) == 0)
		return ;
	bin_paths = ls_bin_paths(arg);
	while (bin_paths != 0)
	{
		if (access(bin_paths->content, X_OK) == 0)
		{
			free(*bin_arg);
			*bin_arg = ft_strdup(bin_paths->content);
			break ;
		}
		tmp = bin_paths->next;
		free(bin_paths->content);
		free(bin_paths);
		bin_paths = tmp;
	}
	clean_str_ls(bin_paths);
}
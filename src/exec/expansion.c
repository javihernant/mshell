#include "exec.h"

int	contains_glob(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] == '*')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

void	expand_globs_aux(t_list **new_args, int *argv_idx, char **argv)
{
	t_list	*tmp;
	t_list	*args;

	args = *new_args;
	while (args != 0)
	{
		argv[*argv_idx] = ft_strdup(args->content);
		*argv_idx += 1;
		tmp = args->next;
		free(args->content);
		free(args);
		args = tmp;
	}
	*new_args = args;
}

void	expand_globs(char *arg, int *argv_idx, char ***ptr_argv, int prev_argv_len)
{
	t_list	*new_args;
	int		new_len;

	if (!contains_glob(arg))
	{
		(*ptr_argv)[*argv_idx] = arg;
		*argv_idx += 1;
		return ;
	}
	new_args = replace_glob(arg);
	new_len = prev_argv_len + (lstlen(new_args) * sizeof(char *));
	if (new_len != prev_argv_len)
		*ptr_argv = ft_realloc(*ptr_argv, prev_argv_len, new_len);
	expand_globs_aux(&new_args, argv_idx, *ptr_argv);
}

char *expand_qstvar_aux(char *line, char *rc)
{
	int		i;
	int		j;
	t_list	*strs;

	i = 0;
	j = 0;
	strs = 0;
	while (line[j] != '\0')
	{
		if (line[j] == '$' && line[j + 1] == '?')
		{
			lstadd_back(&strs, str_slice(line, i, j));
			lstadd_back(&strs, ft_strdup(rc));
			j += 2;
			i = j;
		}
		else
			j++;
	}
	if (j > i)
		lstadd_back(&strs, str_slice(line, i, j));
	free(rc);
	return (joinstrs(strs, 0));
}


char	*expand_qstvar(t_arg *arg, int last_rc)
{
	char	*tmp;
	if (arg->type == ARG_DFLT_SGL)
		return (arg->arg);
	tmp = expand_qstvar_aux(arg->arg, ft_itoa(last_rc));
	free(arg->arg);
	arg->arg = tmp;
	return (tmp);
}
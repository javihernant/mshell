#include "exec.h"

void	process_argsls_aux(t_arg *arg, char ***argvp, int *argvidx, int cnt)
{
	int	i;

	i = *argvidx;
	if (arg->type == ARG_DFLT || arg->type == ARG_DFLT_SGL
		|| arg->type == ARG_DFLT_DBL)
	{
		if (arg->type == ARG_DFLT)
			expand_globs(arg->arg, &i, argvp, sizeof(char *) * (cnt + 1));
		else
		{
			(*argvp)[i] = arg->arg;
			i++;
		}
	}
	else
		set_redir(arg);
	free(arg);
	*argvidx = i;
}

//Converts t_list of args into an array (format required by execve).
//Sets redirections.
char	**process_argsls(t_list *args, int last_rc)
{
	int		cnt;
	char	**argv;
	int		i;
	t_arg	*arg;

	i = 0;
	cnt = lstlen(args);
	argv = malloc(sizeof(char *) * (cnt + 1));
	while (args != 0)
	{
		arg = args->content;
		if (arg->type == ARG_DFLT || arg->type == ARG_DFLT_SGL
			|| arg->type == ARG_DFLT_DBL)
		{
			argv[i] = expand_qstvar(arg, last_rc);
		}
		process_argsls_aux(args->content, &argv, &i, cnt);
		args = next(args);
	}
	argv[i] = 0;
	return (argv);
}

int	*init_pipes(t_list *cmds)
{
	int	pipes_cnt;
	int	*fds;
	int	i;
	int	tmp;

	pipes_cnt = lstlen(cmds) - 1;
	fds = malloc(sizeof(int) * (pipes_cnt * 2 + 2));
	fds[0] = 0;
	fds[(pipes_cnt * 2) + 1] = 1;
	i = 1;
	while (i < pipes_cnt + 1)
	{
		pipe(&fds[i]);
		tmp = fds[i];
		fds[i] = fds[i + 1];
		fds[i + 1] = tmp;
		i += 2;
	}
	return (fds);
}

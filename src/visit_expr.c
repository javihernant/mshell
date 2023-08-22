#include "errors.h"
#include "exprs.h"
#include "lists_fts.h"
#include "strings_fts.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>
#include "globs.h"
#include <sys/wait.h>

int		exec_cmds(t_list *cmds);
void	exec_cmd(t_list *args, int *fds);

int	visit_expr(t_expr *expr)
{
	int	rc;

	if (expr->type == EXPR_PAR)
	{
		return (visit_expr(expr->expr_a));
	}
	else if (expr->type == EXPR_AND)
	{
		rc = visit_expr(expr->expr_a);
		if (rc == 0)
			return (visit_expr(expr->expr_b));
		return (rc);
	}
	else if (expr->type == EXPR_OR)
	{
		rc = visit_expr(expr->expr_a);
		if (rc != 0)
			return (visit_expr(expr->expr_b));
		return (rc);
	}
	else
		return (exec_cmds(expr->cmds));
	return (1);
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

int	exec_cmds(t_list *cmds)
{
	int	rc;
	int	i;
	int	*fds;
	int	cmds_cnt;

	cmds_cnt = lstlen(cmds);
	i = 0;
	fds = init_pipes(cmds);
	while (cmds != 0)
	{
		exec_cmd(cmds->content, &fds[i * 2]);
		cmds = next(cmds);
		i++;
	}
	i = 0;
	while (i < cmds_cnt)
	{
		wait(&rc);
		rc = WEXITSTATUS(rc);
		i++;
	}
	free(fds);
	return (rc);
}

void	set_out_redir(t_arg *arg)
{
	int	fd;

	fd = 1;
	if (arg->type == ARG_OUTAPND)
		fd = open(arg->arg, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
	else
		fd = open(arg->arg, O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
	dup2(fd,1);
}

void	set_in_redir(char *filename)
{
	int	fd;

	fd = open(filename, O_CREAT | O_RDONLY);
	dup2(fd,0);
}

void	set_hdoc_redir(char	*delim)
{
	char	*line;
	int		tmp_fd;
	int		end;

	tmp_fd = open("/tmp/hdoc_tmp", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);

	while (1)
	{
		line = readline("> ");
		if (line == 0)
			break ;
		end = ft_strsequal(line, delim);
		if (!end)
		{
			write(tmp_fd, line, ft_strlen(line));
			write(tmp_fd, "\n", 1);
		}
		free(line);
		if (end)
			break ;
	}
	close(tmp_fd);
	tmp_fd = open("/tmp/hdoc_tmp", O_CREAT | O_RDONLY);
	dup2(tmp_fd,0);
}

void	set_redir(t_arg *arg)
{
	if (arg->type == ARG_OUT || arg->type == ARG_OUTAPND)
		set_out_redir(arg);
	else if (arg->type == ARG_HDOC)
		set_hdoc_redir(arg->arg);
	else if (arg->type == ARG_IN)
		set_in_redir(arg->arg);
	free(arg->arg);
	free(arg);
}

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
//Converts t_list of args into an array (format required by execve). Sets redirections.
char	**process_argsls(t_list *args)
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
			argv[i] = expand_qstvar(arg, 123); //TODO: RC
		}
		process_argsls_aux(args->content, &argv, &i, cnt);
		args = next(args);
	}
	argv[i] = 0;
	return (argv);
}

//TODO
int	is_builtin(char *program)
{
	(void) program;
	return (0);
}

//TODO
int	exec_builtin(char **argv)
{
	(void) argv;
	// free_strarr(argv);
	return (0);
}

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

void	exec_dflt_cmd_aux(char **argv)
{
	char **envp = 0; //TODO: implement global env

	find_binary(&argv[0]);
	execve(argv[0], argv, envp);
	ft_error("Failure at executing program"); //TODO: dont exit, only terminate child process.
}

void	exec_dflt_cmd(char **argv, int *fds)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		if (fds[0] != 0)
		{
			dup2(fds[0], 0);
			close(fds[0]);
		}
		if (fds[1] != 1)
		{
			dup2(fds[1], 1);
			close(fds[1]);
		}
		exec_dflt_cmd_aux(argv);
	}
	else
	{
		if (fds[0] != 0)
			close(fds[0]);
		if (fds[1] != 1)
			close(fds[1]);
		free_strarr(argv);
	}
}

void	exec_cmd(t_list *args, int *fds)
{
	char	**argv;

	argv = process_argsls(args);
	if (argv != 0 && argv[0] != 0)
	{
		if (is_builtin(argv[0]))
		{
			exec_builtin(argv);
		}
		else
		{
			exec_dflt_cmd(argv, fds);
		}
	}
}


// int main()
// {
// 	ls_bin_paths("cat");
// }

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

int	exec_cmds(t_list *cmds);
int	exec_cmd(t_list *args);

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

int	exec_cmds(t_list *cmds)
{
	int	rc;

	rc = 0;
	while (cmds != 0)
	{
		//TODO: pipes
		rc = exec_cmd(cmds->content);
		cmds = next(cmds);

	}
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
	printf("REDIR (%d): %s\n", arg->type, arg->arg);
	//TODO: Free arg->arg ???
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

int	count_nonempty_strs(t_list	*strs)
{
	int	i;

	i = 0;
	while (strs != 0)
	{
		if (((char *)(strs->content))[0] != '\0')
		{
			i++;
		}
		strs = strs->next;
	}
	return (i);
}
//TODO: modify argv array. Skip '' from non-matching patterns.<
void	expand_globs_aux(t_list *new_args, int *argv_idx, char **argv)
{
	t_list	*tmp;

	while (new_args != 0)
	{
		if (((char *)(new_args->content))[0] != '\0')
		{
			argv[*argv_idx] = ft_strdup(new_args->content);
			*argv_idx += 1;
		}
		tmp = new_args->next;
		free(new_args->content);
		free(new_args);
		new_args = tmp;
	}
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
	new_len = prev_argv_len + (count_nonempty_strs(new_args) * sizeof(char *));
	if (new_len != prev_argv_len)
	{
		free(arg);
		*ptr_argv = ft_realloc(*ptr_argv, prev_argv_len, new_len);
		expand_globs_aux(new_args, argv_idx, *ptr_argv);
	}
	else
	{
		clean_str_ls(new_args);
	}
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
			lstadd_back(&strs, rc);
			j += 2;
			i = j;
		}
		else
			j++;
	}
	if (j > i)
		lstadd_back(&strs, str_slice(line, i, j));
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
			argv[i] = expand_qstvar(arg, 123);
			if (arg->type == ARG_DFLT)
				expand_globs(arg->arg, &i, &argv, sizeof(char *) * (cnt + 1));
			else
			{
				argv[i] = arg->arg;
				i++;
			}
		}
		else
			set_redir(arg);
		args = next(args);
	}
	argv[i] = 0;
	return (argv);
}

int	is_builtin(char *program)
{
	(void) program;
	return (0);
}

int	exec_builtin(char **argv)
{
	(void) argv;
	return (0);
}

int	exec_dflt_cmd(char **argv)
{
	int	pid;
	int	rc;
	char	**envp;

	envp = 0;
	pid = fork();
	if (pid == 0)
	{
		execve(argv[0], argv, envp);
		ft_error("Failure at executing program");
	}
	else
	{
		wait(&rc);
		rc = WEXITSTATUS(rc);
		return (rc);
	}
	return (1);
}

int	exec_cmd(t_list *args)
{
	char	**argv;
	int		i;
	int		rc;

	i = 0;
	rc = 0;
	argv = process_argsls(args);
	if (argv != 0 && argv[0] != 0)
	{
		if (is_builtin(argv[0]))
		{
			rc = exec_builtin(argv);
		}
		else
		{
			rc = exec_dflt_cmd(argv);
		}
	}
	//TODO:Clean argv
	while (argv[i] != 0)
	{
		printf("'%s' ", argv[i]);
		i++;
	}
	printf("\n");
	return (rc);
}

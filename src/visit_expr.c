#include "exprs.h"
#include "lists_fts.h"
#include "strings_fts.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>


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
		//TEST: pipes
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
}

//TODO: * and $VARS
char	*extend_arg(char *str)
{
	// char	*new_str;

	// free(str);
	return (str);
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
		if (arg->type == ARG_DFLT)
		{
			argv[i] = extend_arg(arg->arg);
			i++;
		}
		else
			set_redir(arg);
		args = next(args);
	}
	argv[i] = 0;
	return (argv);
}

//This must be executed by child process
int	exec_cmd(t_list *args)
{
	char	**argv;
	int		i;

	i = 0;
	argv = process_argsls(args);
	while (argv[i] != 0)
	{
		printf("%s ", argv[i]);
		i++;
	}
	printf("\n");
	//Test: View content of argv OK
	//Test: indirection OUT and OUTAPND
	//Test: indirection IN and HDOC
	//Test: see that $VARS work
	//Test: see that *works
	return (0);
}

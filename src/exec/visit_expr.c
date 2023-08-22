#include "exec.h"
#include <stdio.h>

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

void	exec_cmd(t_list *args, int *fds, int last_rc)
{
	char	**argv;

	argv = process_argsls(args, last_rc);
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

int	exec_cmds(t_list *cmds, int last_rc)
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
		exec_cmd(cmds->content, &fds[i * 2], last_rc);
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

// int	visit_expr(t_expr *expr, int last_rc)
// {
// 	int	rc;

// 	if (expr->type == EXPR_PAR)
// 		return (visit_expr(expr->expr_a, last_rc));
// 	else if (expr->type == EXPR_AND)
// 	{
// 		rc = visit_expr(expr->expr_a, last_rc);
// 		if (rc == 0)
// 			return (visit_expr(expr->expr_b, rc));
// 		return (rc);
// 	}
// 	else if (expr->type == EXPR_OR)
// 	{
// 		rc = visit_expr(expr->expr_a, last_rc);
// 		if (rc != 0)
// 			return (visit_expr(expr->expr_b, rc));
// 		return (rc);
// 	}
// 	else
// 		return (exec_cmds(expr->cmds, last_rc));
// 	return (1);
// }

int	visit_expr(t_expr *expr, int last_rc)
{
	int	rc;

	if (expr->type == EXPR_PAR)
		rc = visit_expr(expr->expr_a, last_rc);
	else if (expr->type == EXPR_AND)
	{
		rc = visit_expr(expr->expr_a, last_rc);
		if (rc == 0)
			rc = visit_expr(expr->expr_b, rc);
		else
			free_expr(expr->expr_b);
	}
	else if (expr->type == EXPR_OR)
	{
		rc = visit_expr(expr->expr_a, last_rc);
		if (rc != 0)
			rc = visit_expr(expr->expr_b, rc);
		else
			free_expr(expr->expr_b);
	}
	else
		rc = exec_cmds(expr->cmds, last_rc);
	free(expr);
	return (rc);
}
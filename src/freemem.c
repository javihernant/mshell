#include "exprs.h"
#include "lists_fts.h"
#include <stdlib.h>

void	free_arg(t_arg *arg)
{
	char	*str;

	str = arg->arg;
	free(arg);
	free(str);
}

void	free_cmd(t_list *args)
{
	t_list	*tmp;

	while (args != 0)
	{
		tmp = args->next;
		free_arg(args->content);
		free(args);
		args = tmp;
	}
}

void	free_cmds(t_list *cmds)
{
	t_list	*tmp;

	while (cmds != 0)
	{
		free_cmd(cmds->content);
		tmp = cmds->next;
		free(cmds);
		cmds = tmp;
	}
}

//Used to free t_expr that are not executed (ie. `cmd1 && cmd2` when cmd2 if cmd1 fails)
void	free_expr(t_expr *expr)
{
	if (expr->type == EXPR_PAR)
		free_expr(expr->expr_a);
	else if (expr->type == EXPR_AND || expr->type == EXPR_OR)
	{
		free_expr(expr->expr_a);
		free_expr(expr->expr_b);
	}
	else
		free_cmds(expr->cmds);
	free(expr);
}

#include "exprs.h"
#include "interpret.h"

t_expr	*mk_expr(int type, t_expr *expra, t_expr *exprb, t_list *cmds)
{
	t_expr	*new_expr;

	new_expr = malloc(sizeof(t_expr));
	new_expr->type = type;
	new_expr->expr_a = expra;
	new_expr->expr_b = exprb;
	new_expr->cmds = 0;
	if (type == EXPR_CMDS)
		new_expr->cmds = cmds;
	return (new_expr);
}

t_token	*peek_tkn(t_list *tokens)
{
	t_token	*tkn;

	tkn = 0;
	if (tokens != 0)
	{
		tkn = tokens->content;
	}
	return (tkn);
}

void	advance_tkn_ls(t_list **tokens, t_token **tkn)
{
	t_token	*tmp;

	tmp = (t_token *)(*tokens)->content;
	*tokens = next(*tokens);
	free(tmp);
	*tkn = peek_tkn(*tokens);
}

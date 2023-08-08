#include "exprs.h"
#include "lexer.h"
#include "lists_fts.h"
#include "errors.h"
#include <stdlib.h>
#include "tests.h"

t_expr	*mk_expr(int type, t_expr *expra, t_expr *exprb, t_list *args)
{
	t_expr	*new_expr;

	new_expr = malloc(sizeof(t_expr));
	new_expr->type = type;
	new_expr->expr_a = expra;
	new_expr->expr_b = exprb;
	new_expr->args = 0;
	if (type == EXPR_ARGS)
		new_expr->args = args;
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
	*tokens = next(*tokens);
	*tkn = peek_tkn(*tokens);
}

t_expr	*args(t_list **tokens)
{
	t_token	*tkn;
	t_expr	*expr;

	expr = 0;
	tkn = peek_tkn(*tokens);
	if (tkn != 0)
	{
		expr = mk_expr(EXPR_ARGS, 0, 0, tkn->args);
		advance_tkn_ls(tokens, &tkn);
	}
	return (expr);
}

t_expr	*ops(t_list **tokens);

t_expr	*pars(t_list **tokens)
{
	t_token	*tkn;
	t_expr	*expr;

	expr = 0;
	tkn = peek_tkn(*tokens);
	if (tkn != 0)
	{
		if (tkn->type == TKN_LPAR)
		{
			advance_tkn_ls(tokens, &tkn);
			expr = ops(tokens);
			tkn = peek_tkn(*tokens);
			if (tkn != 0 && tkn->type == TKN_RPAR)
			{
				advance_tkn_ls(tokens, &tkn);
				return (mk_expr(EXPR_PAR, expr, 0, 0));
			}
			else
				ft_error("Expected ')'");
		}
		else
			return (args(tokens));
	}
	return (expr);
}

t_expr	*ops(t_list **tokens)
{
	t_token	*tkn;
	t_expr	*expra;
	int		expr_type;

	expra = pars(tokens);
	expr_type = expra->type;
	tkn = peek_tkn(*tokens);
	if (tkn != 0)
	{
		if (tkn->type == TKN_AND || tkn->type == TKN_OR)
		{
			if (tkn->type == TKN_AND)
				expr_type = EXPR_AND;
			else
				expr_type = EXPR_OR;
			advance_tkn_ls(tokens, &tkn);
			return (mk_expr(expr_type, expra, ops(tokens), 0));
		}
		else if (tkn->type == TKN_RPAR)
		{
		}
		else
			ft_error("Expected && or || operator");
	}
	return (expra);
}

t_expr	*gen_expr(char *line)
{
	t_list	*tokens;

	tokens = get_tokens(line);
	return (ops(&tokens));
}
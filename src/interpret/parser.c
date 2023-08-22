#include "interpret.h"

t_expr	*cmds(t_list **tokens)
{
	t_token	*tkn;
	t_expr	*expr;

	expr = 0;
	tkn = peek_tkn(*tokens);
	if (tkn != 0)
	{
		expr = mk_expr(EXPR_CMDS, 0, 0, tkn->cmds);
		advance_tkn_ls(tokens, &tkn);
	}
	return (expr);
}

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
			return (cmds(tokens));
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
#include "exprs.h"
#include "tests.h"
#include <stdio.h>
void	print_expr(t_expr *expr, int lvl)
{
	while (lvl--)
		printf("\t");
	if (expr->type == EXPR_PAR) {
		printf("PARS\n");
		print_expr(expr->expr_a, lvl + 1);
	}
	else if (expr->type == EXPR_AND) {
		printf("AND\n");

		print_expr(expr->expr_a, lvl+1);
		print_expr(expr->expr_b, lvl + 1);
	}
	else if (expr->type == EXPR_OR) {
		printf("OR\n");
		print_expr(expr->expr_a, lvl + 1);
		print_expr(expr->expr_b, lvl + 1);
	}
	else if (expr->type == EXPR_CMDS)
	{
		printf("LIST OF CMDS:\n");
		print_cmds(expr->cmds);
	}
}

void	test_expr_aux(char *line)
{
	t_expr	*expr;

	expr = gen_expr(line);
	print_expr(expr, 0);
}

// void	test_expr()
// {
// 	// test_skip_char();
// 	// test_parse_arg();
// 	//test_parse_args();
// 	// test_get_tokens();
	
// 	test_expr_aux("cmd1 \"hello\\\"'dear how are you?\"    	\"snd arg\" && (cm2 \")\" || cmd3 arg1 arg2 ) || cmd4");
// 	// test_expr_aux("cm2 || cmd3 arg1 arg2  || cmd4");
// }

// int	main()
// {
// 	test_expr_aux("cmd1 \"ignore | > << >>\" arg2 | cmd2 arg1 > out1 >> out2 < in1 << in2 | cmd3 && cmd4");

// }
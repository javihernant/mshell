#include "interpret.h"
#include "exec.h"
#include <readline/readline.h>

//TODO: fix: shell crashes with empty line.
int	main(void)
{
	char	*line;
	t_expr	*expr;
	int		rc;

	rc = 0;
	while (1)
	{
		line = readline("> ");
		if (line == 0)
			return (0);
		expr = gen_expr(line);
		if (expr != 0)
			rc = visit_expr(expr, rc);
		free(line);
	}
	return (0);
}
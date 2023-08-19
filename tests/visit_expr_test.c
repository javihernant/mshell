#include "lists_fts.h"
#include "tests.h"
#include <stdio.h>

void	test_exec_cmd(char *str)
{
	int		rc;
	t_list	*args;
	int		i;

	args = parse_cmd(str, &i);
	rc = exec_cmd(args);
	printf("RC:%d", rc);
}

int main()
{
	// test_exec_cmd("ls *.o");
	test_exec_cmd("oijfireofije *.o");
}
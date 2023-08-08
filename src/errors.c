#include <stdio.h>
#include <stdlib.h>

void	ft_error(char *msg)
{
	printf("ERROR: %s\n", msg);
	printf("exiting...");
	exit(1);
}
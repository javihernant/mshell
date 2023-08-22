#include <stdio.h>
#include <stdlib.h>

void	ft_error(char *msg)
{
	perror(msg);
	exit(1);
}
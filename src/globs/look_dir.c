#include <dirent.h>
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lists_fts.h"
#include "strings_fts.h"
#include "globs.h"

t_list	*replace_glob(char *pat)
{
	char			*cwd;
	DIR				*dir;
	struct dirent	*file;
	t_list			*strs;

	strs = 0;
	cwd = getcwd(0, 0);
	dir = opendir(cwd);
	if (dir == 0)
		ft_error(strerror(errno));
	while (1)
	{
		file = readdir(dir);
		if (file == 0)
			break ;
		if (!ft_strsequal(file->d_name, "..")
			&& !ft_strsequal(file->d_name, "."))
		{
			if (check_pattern(pat, file->d_name))
				lstadd_back(&strs, ft_strdup(file->d_name));
		}
	}
	if (strs == 0)
		lstadd_back(&strs, ft_strdup(pat));
	free(pat);
	free(cwd);
	closedir(dir);
	return (strs);
}

// int main()
// {
// 	char *str = replace_glob("*.o");
// 	printf("%s", str);
// }
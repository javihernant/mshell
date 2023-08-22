#ifndef EXEC_H
# define EXEC_H

#include "errors.h"
#include "exprs.h"
#include "lists_fts.h"
#include "strings_fts.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>
#include "globs.h"
#include <sys/wait.h>

t_list	*ls_paths_aux(char *path_var);
t_list	*ls_paths(void);
t_list	*ls_bin_paths(char *arg);
void	find_binary(char **bin_arg);
void	process_argsls_aux(t_arg *arg, char ***argvp, int *argvidx, int cnt);
char	**process_argsls(t_list *args, int last_rc);
int		*init_pipes(t_list *cmds);
int		contains_glob(char *arg);
void	expand_globs_aux(t_list **new_args, int *argv_idx, char **argv);
void	expand_globs(char *arg, int *argvidx, char ***pargv, int argv_len);
char	*expand_qstvar_aux(char *line, char *rc);
char	*expand_qstvar(t_arg *arg, int last_rc);
void	set_out_redir(t_arg *arg);
void	set_in_redir(char *filename);
void	set_hdoc_redir(char	*delim);
void	set_redir(t_arg *arg);
void	exec_dflt_cmd_aux(char **argv);
void	exec_dflt_cmd(char **argv, int *fds);
void	exec_cmd(t_list *args, int *fds, int last_rc);
int		exec_cmds(t_list *cmds, int last_rc);
int		visit_expr(t_expr *expr, int last_rc);
int		is_builtin(char *program);
int		exec_builtin(char **argv);

#endif
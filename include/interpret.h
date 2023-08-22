#ifndef INTERPRET_H
# define INTERPRET_H
# include "errors.h"
# include "strings_fts.h"
# include "lists_fts.h"
# include "exprs.h"
# include "tests.h"
# include <stdlib.h>

int		skip_char(char *line, int i, int j);
int		is_concat_op(char *line, int idx);
int		is_end_of_arg(char first_c, char *line, int i, int j);
char	*parse_str_arg(char *line, int *idx);
t_arg	*parse_arg(char *line, int *idx);
int		is_par(char c);
int		arg_type(char *line, int idx);
t_arg	*mk_arg(int type, char *str);
t_arg	*parse_other_arg(char *line, int *idx);
t_list	*parse_cmd(char *line, int *idx);
t_list	*parse_cmds(char *line, int *idx);
t_token	*mk_tkn(int type, t_list *lst);
t_token	*gen_par_tkn(char *line, int *idx);
t_token	*gen_op_tkn(char *line, int *idx);
t_token	*produce_token(char *line, int *idx);
t_list	*get_tokens(char *line);
t_expr	*mk_expr(int type, t_expr *expra, t_expr *exprb, t_list *cmds);
t_token	*peek_tkn(t_list *tokens);
void	advance_tkn_ls(t_list **tokens, t_token **tkn);
t_expr	*cmds(t_list **tokens);
t_expr	*pars(t_list **tokens);
t_expr	*ops(t_list **tokens);
t_expr	*gen_expr(char *line);
char	*expand_envs(char *line);
void	skip_single_qt(char *line, int *idx);
char	*get_env_var(char *line, int *idx);
char	*parse_env_var(char *line, int *idx);

#endif
#include "lists_fts.h"
#include "tests.h"
#include "exprs.h"
#include <stdio.h>


/*
    parse_lexeme1
    IN:"cmd \"skip ' and \\\" and && and || \" arg2 && cmd arg1 &&   cmd arg1 arg2 || cmd arg1"
    OUT: ["cmd", "skip ' and \\\" and && and || \", "arg2"]
*/

void	test_skip_char(void)
{
	char	*skip1;
	char	*dont_skip1;
	char	*skip2;

	skip1 = "\\\"";
	dont_skip1 = "\\\\\"";
	skip2 = "\\\\\\\"";
	printf("%s: skip_char:%d\n", skip1, skip_char(skip1, 0,1));
	printf("%s: skip_char:%d\n", dont_skip1, skip_char(dont_skip1, 0,2));
	printf("%s: skip_char:%d\n", skip2, skip_char(skip2, 0,3));
}
// void	test_parse_arg_aux(char *in)
// {
// 	char	*out;
// 	int		idx;

// 	idx = 0;
// 	out = parse_arg(in, &idx);
// 	printf("IN:%s ||| OUT:%s ||| IDX:%d\n", in, out, idx);
// }

// void	test_parse_arg()
// {
// 	test_parse_arg_aux("arg1 this is ignored, right?");

// 	test_parse_arg_aux("\"hello\\\"'dear how are you?\"");
// 	test_parse_arg_aux("\'hello\\\'\"dear how are you?\'");
// 	test_parse_arg_aux("\"hello\\\\\"dear how are you?\"");
// 	test_parse_arg_aux("arg1&& ");
// 	test_parse_arg_aux("arg1||");
// }

void	print_lst(t_list *lst)
{
	printf("[ ");
	while (lst != 0)
	{
		printf("\"%s\",\n", (char *)lst->content);
		lst = lst->next;
	}
	printf(" ]\n");
}

// void	test_parse_args_aux(char *line)
// {
// 	t_list	*lst;
// 	int i = 0;
// 	lst = parse_args(line, &i);
// 	printf("IN:%s\n", line);
// 	printf("OUT:");
// 	print_lst(lst);
// }

// void	test_parse_args()
// {
// 	test_parse_args_aux("cmd1 \"hello\\\"'dear how are you?\"    	\"snd arg\" && cm2 ");
// }

void	print_token(t_token *token)
{
	if (token->type == TKN_AND) {
		printf("&&\n");
	} else if (token->type == TKN_OR){
		printf("||\n");

	} else if (token->type == TKN_LPAR){
		printf("(\n");

	}else if (token->type == TKN_RPAR){
		printf(")\n");
	}else if (token->type == TKN_ARGS){
		print_lst(token->args);
	} 
}

void	test_get_tokens_aux(char *line)
{
	t_list *tokens;
	int		i;

	printf("IN:%s\n", line);
	printf("OUT\n");
	i = 0;
	tokens = get_tokens(line);
	while (tokens != 0)
	{
		printf("TOKEN %d:\n",i);
		print_token(tokens->content);
		i++;
		tokens = tokens->next;
	}
}

void	test_get_tokens()
{
	test_get_tokens_aux("cmd1 \"hello\\\"'dear how are you?\"    	\"snd arg\" && (cm2 || cmd3 arg1 arg2 )");

}

void	print_cmd(t_list *args)
{
	t_arg	*curr_arg;

	printf("[ ");
	while (args != 0)
	{
		curr_arg = args->content;
		if (curr_arg->type == ARG_DFLT)
			printf("\"%s\",\n", curr_arg->arg);
		else
			printf("ARG_TYPE:%d\t%s\n",curr_arg->type, curr_arg->arg);
		args = args->next;
	}
	printf(" ]\n");
}

void	print_cmds(t_list *cmds)
{
	int	i;

	i = 0;
	while (cmds != 0) 
	{
		printf("CMD N%d", i);
		print_cmd(cmds->content);
		cmds = cmds->next;
		i++;
	}
}

void test_parse_cmds(char *line)
{
	t_list	*cmds;
	int		idx;

	idx = 0;
	cmds = parse_cmds(line, &idx);
	print_cmds(cmds);
}

int	main()
{
	test_parse_cmds("cmd1 \"ignore | > << >>\" arg2 | cmd2 arg1 > out1 >> out2 < in1 << in2 | cmd3 && cmd4");
}

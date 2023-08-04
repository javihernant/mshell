#include "tests.h"
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
void	test_parse_arg_aux(char *in)
{
	char	*out;
	int		idx;

	idx = 0;
	out = parse_arg(in, &idx);
	printf("IN:%s ||| OUT:%s ||| IDX:%d\n", in, out, idx);
}

void	test_parse_arg()
{
	test_parse_arg_aux("arg1 this is ignored, right?");

	test_parse_arg_aux("\"hello\\\"'dear how are you?\"");
	test_parse_arg_aux("\'hello\\\'\"dear how are you?\'");
	test_parse_arg_aux("\"hello\\\\\"dear how are you?\"");
	test_parse_arg_aux("arg1&& ");
	test_parse_arg_aux("arg1||");
}

int main(void)
{
	// test_skip_char();
	test_parse_arg();
}
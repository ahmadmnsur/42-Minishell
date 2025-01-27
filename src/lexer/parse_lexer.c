#include "../../minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_special_char(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '\'' || c == '\"');
}

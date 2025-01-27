#include "../../minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_special_char(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '\'' || c == '\"');
}

void	skip_spaces(const char *str, int *i)
{
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
}

void	parse_string(t_lexer **lexer, const char *str)
{
	int		i;
	t_lexer	*new;

	i = 0;
	while (i < (int)ft_strlen(str))
	{
		if (is_whitespace(str[i]))
			skip_spaces(str, &i);
		else if (is_special_char(str[i]))
		{
			if (str[i] == '<')
				handle_in_or_heredoc(lexer, str, &i);
			else if (str[i] == '\'' || str[i] == '\"')
				handle_quotes(lexer, str, &i, str[i]);
			else if (str[i] == '|')
                handle_pipe(lexer, &i);
			else if (str[i] == '>')
				handle_out_or_append(lexer, str, &i);
		}
		else
			handle_normal_string(lexer, str, &i);
	}
	new = create_new_lexer_node(NULL, TOKEN_EOF, -1, -1);
	(lexer_add_back(lexer, new), add_index_token(lexer));
}

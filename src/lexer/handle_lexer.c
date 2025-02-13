#include "../../minishell.h"

void	handle_normal_string(t_lexer **lexer, const char *str, int *i)
{
	int		start;
	int		end;
	char	*sub;
	t_lexer	*new;

	start = *i;
	end = start;
	while (str[end] && !special_char(str[end]) && !whitespacee(str[end]))
		end++;
	sub = ft_substr(str, start, end - start);
	new = create_new_lexer_node(sub, TOKEN_WORD, NO_QUOTE, 1);
	if (str[end] == '"' || str[end] == '\'')
		new->space = 0;
	add_lexer_node_back(lexer, new);
	free(sub);
	*i = end;
}

void	handle_pipe(t_lexer **lexer, int *i)
{
	t_lexer	*new;

	new = create_new_lexer_node(NULL, TOKEN_PIPE, -1, 1);
	add_lexer_node_back(lexer, new);
	(*i)++;
}

void	handle_in_or_heredoc(t_lexer **lexer, const char *str, int *i)
{
	if (str[*i + 1] && str[*i + 1] == '<')
		add_lexer_and_increment_i(lexer, TOKEN_HEREDOC, i, 2);
	else
		add_lexer_and_increment_i(lexer, TOKEN_REDIRECT_IN, i, 1);
}

void	handle_out_or_append(t_lexer **lexer, const char *str, int *i)
{
	if (str[*i + 1] && str[*i + 1] == '>')
		add_lexer_and_increment_i(lexer, TOKEN_APPEND, i, 2);
	else
		add_lexer_and_increment_i(lexer, TOKEN_REDIRECT_OUT, i, 1);
}

void	handle_quotes(t_lexer **lexer, const char *str, int *i, char quote)
{
	char			*sub;
	t_lexer			*new;
	t_quote_type	type;
	int				len;

	type = get_quote_type(quote);
	len = get_quote_length(str, *i, quote);
	sub = ft_substr(str, *i + 1, len - 1);
	new = create_new_lexer_node(sub, TOKEN_WORD, type, 1);
	if (!whitespacee(str[*i + len + 1]) && str[*i + len + 1] != '<'
		&& str[*i + len + 1] != '>' && str[*i + len + 1] != '|')
		new->space = 0;
	add_lexer_node_back(lexer, new);
	free(sub);
	*i += len + 1;
}

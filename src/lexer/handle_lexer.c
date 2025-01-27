#include "../../minishell.h"

void    handle_normal_string(t_lexer **lexer, const char *str, int *i)
{
    int     start;
    int     end;
    char    *sub;
    t_lexer  *new;

    start = *i;
    end = start;
    while (str[end] && !is_special_char(str[end]) && !is_whitespace(str[end]))
        end++;
    sub = ft_substr(str, start, end - start);
    new = create_new_lexer_node(sub, TOKEN_WORD, NO_QUOTE, 1);
    if (str[end] == '"' || str[end] == '\'')
        new->space = 0;
    lexer_add_back(lexer, new);
    free(sub);
    *i = end;
}

void	handle_pipe(t_lexer **lexer, int *i)
{
	t_lexer	*new;

	new = create_new_lexer_node(NULL, TOKEN_PIPE, -1, 1);
	lexer_add_back(lexer, new);
	(*i)++;
}

void	handle_in_or_heredoc(t_lexer **lexer, const char *str, int *i)
{
	t_lexer	*new;

	if (str[*i + 1] && str[*i + 1] != '<')
	{
		new = create_new_lexer_node(NULL, TOKEN_REDIRECT_IN, -1, 1);
		(*i)++;
	}
	else
	{
        new = create_new_lexer_node(NULL, TOKEN_HEREDOC, -1, 1);
		(*i) += 2;
	}
	lexer_add_back(lexer, new);
}

void	handle_out_or_append(t_lexer **lexer, const char *str, int *i)
{
	t_lexer	*new;

	if (str[*i + 1] && str[*i + 1] != '>')
	{
		new = create_new_lexer_node(NULL, TOKEN_REDIRECT_OUT, -1, 1);
		(*i)++;
	}
	else
	{
		new = create_new_lexer_node(NULL, TOKEN_REDIRECT_OUT, -1, 1);
		(*i)++;
        new = create_new_lexer_node(NULL, TOKEN_APPEND, -1, 1);
		(*i) += 2;
	}
	lexer_add_back(lexer, new);
}

void handle_quotes(t_lexer **lexer, const char *str, int *i, char quote)
{
    char        *sub;
    t_lexer     *new;
    t_quote_type type;
    int         len;

    type = get_quote_type(quote);
    len = get_quote_length(str, *i, quote);
    sub = ft_substr(str, *i + 1, len - 1);
    new = create_new_lexer_node(sub, TOKEN_WORD, type, 1);
    if (!is_whitespace(str[*i + len + 1]) && str[*i + len + 1] != '<'
        && str[*i + len + 1] != '>' && str[*i + len + 1] != '|')
        new->space = 0;
    lexer_add_back(lexer, new);
    free(sub);
    *i += len + 1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:00:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 16:00:51 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

void	cat_heredoc_word(t_lexer **lexer, t_lexer *current)
{
	t_lexer	*tmp;
	t_lexer	*to_del;

	to_del = NULL;
	if (!current || (current && current->space))
		return ;
	tmp = *lexer;
	while (tmp != current)
		tmp = tmp->next;
	while (tmp && tmp->next && !(tmp->next->space))
	{
		to_del = tmp->next;
		tmp->str = ft_strjoin(tmp->str, to_del->str);
		tmp->space = 1;
		tmp->quote_type = SINGLE_QUOTES;
		free_lexer_node(lexer, to_del);
	}
	if (tmp && tmp->next)
	{
		to_del = tmp->next;
		tmp->str = ft_strjoin(tmp->str, to_del->str);
		tmp->space = 1;
		tmp->quote_type = SINGLE_QUOTES;
		free_lexer_node(lexer, to_del);
	}
}

void	cat_heredoc_words(t_parser *parser)
{
	t_lexer	*tmp;

	tmp = parser->tokens;
	if (!parser || !(parser->tokens))
		return ;
	while (tmp)
	{
		if (tmp->token == TOKEN_HEREDOC)
		{
			cat_heredoc_word(&(parser->tokens), tmp->next);
			if (tmp->next && tmp->next->next)
				tmp = tmp->next->next;
			else
				break;
			continue ;
		}
		else
			tmp = tmp->next;
	}
}

char	*create_heredoc_file_and_redirect(char **delimiters, t_tools *tools,
	t_quote_type quote_type, int i)
{
	t_lexer	*heredoc_lex;
	char	*filename;
	char	*file_num;
	int		fd;
	t_lexer	*tmp;

	file_num = ft_itoa(i);
	filename = ft_strjoin(ft_strdup("/tmp/heredoc_tmp.txt_"), file_num);
	heredoc_lex = render_input_to_lexer(delimiters[i]);
	update_heredoc_words(heredoc_lex, tools, quote_type);
	tmp = heredoc_lex;
	fd = open_file(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return ((free(filename), free(file_num)), NULL);
	while (tmp)
	{
		ft_putstr_fd(tmp->str, fd);
		tmp = tmp->next;
	}
	(free_lexer(&heredoc_lex), close(fd));
	return (free(file_num), filename);
}


t_lexer	*render_input_to_lexer(char *delimiter)
{
	t_lexer	*lexer;
	t_lexer	*new_node;
	char	*line;
	int		delimiter_len;

	lexer = NULL;
	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("");
		if (!line)
		{
			break;
		}
		if (ft_strncmp(line, delimiter, delimiter_len) == 0)
			return (free(line), lexer);
		line = ft_strjoin(line, "\n");
		new_node = create_new_lexer_node(line, TOKEN_WORD, NO_QUOTE, 1);
		(add_lexer_node_back(&lexer, new_node), free(line));
	}
	return (lexer);
}

void	update_heredoc_words(t_lexer *heredoc_lex, t_tools *tools,
	t_quote_type quote_type)
{
	t_lexer	*tmp;
	char	*new_str;

	new_str = NULL;
	if (!heredoc_lex)
		return ;
	tmp = heredoc_lex;
	if (quote_type == SINGLE_QUOTES || quote_type == DOUBLE_QUOTES)
		return ;
	while (tmp)
	{
		if (ft_strchr(tmp->str, '$'))
		{
			new_str = string_converter(tmp, tools);
			if (tmp->str)
				free(tmp->str);
			tmp->str = ft_strdup(new_str);
		}
		tmp = tmp->next;
	}
	if (new_str)
		free(new_str);
}

int	get_delimiters_count(char **delimiters)
{
	int	count;

	count = 0;
	while (delimiters[count])
		count++;
	return (count);
}
//testt
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:13:51 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 14:13:51 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_lexer	*find_current_node(t_lexer *lexer, t_lexer *current)
{
	while (lexer && lexer != current)
		lexer = lexer->next;
	return (lexer);
}

static void	append_next_token(t_lexer **lexer, t_lexer *node)
{
	t_lexer	*to_del;

	to_del = node->next;
	node->str = ft_strjoin(node->str, to_del->str);
	node->space = 1;
	node->quote_type = SINGLE_QUOTES;
	free_lexer_node(lexer, to_del);
}

void	cat_heredoc_word(t_lexer **lexer, t_lexer *current)
{
	t_lexer	*tmp;

	if (!current || current->space)
		return ;
	tmp = find_current_node(*lexer, current);
	while (tmp && tmp->next && !tmp->next->space)
		append_next_token(lexer, tmp);
	if (tmp && tmp->next)
		append_next_token(lexer, tmp);
}

void	cat_heredoc_words(t_parser *parser)
{
	t_lexer	*tmp;

	if (!parser || !parser->tokens)
		return ;
	tmp = parser->tokens;
	while (tmp)
	{
		if (tmp->token == TOKEN_HEREDOC)
		{
			cat_heredoc_word(&(parser->tokens), tmp->next);
			if (tmp->next && tmp->next->next)
				tmp = tmp->next->next;
			else
				break ;
			continue ;
		}
		tmp = tmp->next;
	}
}

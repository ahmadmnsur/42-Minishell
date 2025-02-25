/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:52:11 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:52:11 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static unsigned int	count_lexer_nodes(t_lexer *lexer)
{
	t_lexer			*tmp_lexer;
	unsigned int	nb_nodes;

	nb_nodes = 0;
	tmp_lexer = lexer;
	while (tmp_lexer)
	{
		nb_nodes++;
		tmp_lexer = tmp_lexer->next;
	}
	return (nb_nodes);
}

void	set_tokens_lexer(t_parser **parser, t_lexer *lexer)
{
	unsigned int	nb_nodes;

	if (!parser || !lexer)
		return ;
	nb_nodes = count_lexer_nodes(lexer);
	assign_tokens_to_parser(parser, lexer, nb_nodes);
}

char	*get_token(t_tokens token)
{
	if (token == TOKEN_REDIRECT_IN)
		return (ft_strdup("<"));
	if (token == TOKEN_REDIRECT_OUT)
		return (ft_strdup(">"));
	if (token == TOKEN_HEREDOC)
		return (ft_strdup("<<"));
	if (token == TOKEN_APPEND)
		return (ft_strdup(">>"));
	return (ft_strdup(""));
}

void	ft_tokens_join(t_lexer **tmp, t_lexer **to_del)
{
	(*to_del) = (*tmp)->next;
	if (ft_strlen((*tmp)->str) == 1 && (*tmp)->str[0] == '$'
		&& (*tmp)->quote_type == NO_QUOTE)
	{
		free((*tmp)->str);
		(*tmp)->str = ft_strdup((*tmp)->next->str);
	}
	else
		(*tmp)->str = ft_strjoin((*tmp)->str, (*tmp)->next->str);
}

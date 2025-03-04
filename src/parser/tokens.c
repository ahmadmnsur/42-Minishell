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

void	set_tokens_lexer(t_parser **parser, t_lexer *lexer)
{
	t_lexer			*tmp;
	unsigned int	nb_nodes;

	if (!parser || !lexer)
		return ;
	nb_nodes = 0;
	tmp = lexer;
	while (tmp)
	{
		nb_nodes++;
		tmp = tmp->next;
	}
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

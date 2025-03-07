/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_limiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:51:35 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:51:35 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
//recursively count the number of heredocs
static int	get_nb_of_heredocs(t_lexer *redirs)
{
	if (!redirs)
		return (0);
	if (redirs->token == TOKEN_HEREDOC)
		return (1 + get_nb_of_heredocs(redirs->next));
	return (get_nb_of_heredocs(redirs->next));
}

//recursively fill the hd_limiters array
static void	fill_hd_limiters(t_lexer *redirs, char **limiters, int *i)
{
	if (!redirs)
		return ;
	if (redirs->token == TOKEN_HEREDOC && redirs->next)
	{
		limiters[*i] = ft_strdup(redirs->next->str);
		(*i)++;
	}
	fill_hd_limiters(redirs->next, limiters, i);
}

//set the hd_limiters array in the parser node
static void	set_hd_limiters_in_node(t_parser *node)
{
	int	nb_hd;
	int	i;

	if (!node || !node->redirects)
		return ;
	nb_hd = get_nb_of_heredocs(node->redirects);
	node->hd_delimiters = malloc(sizeof(char *) * (nb_hd + 1));
	if (!node->hd_delimiters)
		return ;
	i = 0;
	fill_hd_limiters(node->redirects, node->hd_delimiters, &i);
	node->hd_delimiters[i] = NULL;
}

//recursively set the hd_limiters array in the parser
void	set_hd_limiter_parser(t_parser *parser)
{
	if (!parser)
		return ;
	set_hd_limiters_in_node(parser);
	set_hd_limiter_parser(parser->next);
}

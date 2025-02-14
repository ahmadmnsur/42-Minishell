/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:51:59 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:51:59 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	add_to_redirects(t_lexer **redirects, t_lexer *redirection,
		t_lexer *target)
{
	t_lexer	*new_node;

	new_node = create_new_lexer_node(redirection->str, redirection->token,
			redirection->quote_type, redirection->space);
	if (!*redirects)
		*redirects = new_node;
	else
		add_lexer_node_back(redirects, new_node);
	new_node = create_new_lexer_node(target->str, target->token,
			target->quote_type, target->space);
	add_lexer_node_back(redirects, new_node);
}

void	init_one_redirect(t_lexer **redirects, t_lexer **lexer)
{
	t_lexer	*tmp;
	t_lexer	*to_del;
	t_lexer	*next;

	if (!lexer || !*lexer)
		return ;
	tmp = *lexer;
	while (tmp != NULL)
	{
		if (is_redirection_token(tmp->token))
		{
			to_del = tmp;
			next = to_del->next;
			add_to_redirects(redirects, to_del, next);
			if (next != NULL)
				tmp = next->next;
			else
				tmp = NULL;
			free_lexer_node(lexer, to_del);
			if (next != NULL)
				free_lexer_node(lexer, next);
		}
		else
			tmp = tmp->next;
	}
}

void	init_redirections(t_parser *parser)
{
	t_parser	*curr;

	if (!parser)
		return ;
	curr = parser;
	while (curr != NULL && curr->tokens != NULL)
	{
		init_one_redirect(&curr->redirects, &curr->tokens);
		curr = curr->next;
	}
}

void	free_lexer_node(t_lexer **lexer, t_lexer *to_del)
{
	t_lexer	**indirect;

	if (!lexer || !*lexer || !to_del)
		return ;
	indirect = lexer;
	while (*indirect && *indirect != to_del)
		indirect = &((*indirect)->next);
	if (*indirect == to_del)
	{
		*indirect = to_del->next;
		if (to_del->str)
			free(to_del->str);
		free(to_del);
		to_del = NULL;
	}
}

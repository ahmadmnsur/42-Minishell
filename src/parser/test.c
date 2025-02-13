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

int	validate_pipes_and_words(t_parser *head)
{
	t_parser	*current;
	t_lexer		*lexer_node;

	if (head == NULL)
		return (1);
	current = head;
	while (current != NULL)
	{
		if (current->tokens == NULL)
			return (0);
		lexer_node = current->tokens;
		while (lexer_node != NULL)
		{
			if (lexer_node->token == TOKEN_WORD)
				break ;
			lexer_node = lexer_node->next;
		}
		if (lexer_node == NULL && current->next != NULL)
			return (0);
		current = current->next;
	}
	return (1);
}

int	check_unclosed_quotes(char *str)
{
	int		i;
	char	quote;

	if (!str)
		return (0);
	quote = 0;
	i = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		i++;
	}
	return (quote);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:22:29 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 14:22:29 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_empty_split(t_lexer **current, t_lexer *to_del,
	char ***split, char **str)
{
	free_lexer_node(current, to_del);
	if (*split && !(**split))
		free_split(*split);
	free(*str);
}

void	handle_simple_string(t_lexer *to_del, char ***split, char **str)
{
	if (to_del->str)
		free(to_del->str);
	to_del->str = ft_strdup(*str);
	free_split(*split);
	free(*str);
}

void	insert_at(t_lexer **lexer, t_lexer *current, char *str, int space)
{
	t_lexer	*tmp;
	t_lexer	*new;

	if (!lexer || !*lexer || !current)
		return ;
	tmp = *lexer;
	while (tmp != current)
		tmp = tmp->next;
	if (!tmp)
		return ;
	new = create_new_lexer_node(str, TOKEN_WORD, NO_QUOTE, space);
	new->next = tmp->next;
	tmp->next = new;
}

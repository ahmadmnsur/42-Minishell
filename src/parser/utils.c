/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:52:24 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:52:24 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	insert_split_tokens(t_lexer **current, t_lexer *to_del,
		char **split, int count)
{
	int	i;

	i = count - 1;
	while (i >= 0)
	{
		if (i == count - 1)
			insert_at(current, to_del, split[i], to_del->space);
		else
			insert_at(current, to_del, split[i], 1);
		i--;
	}
}

static void	free_update_resources(t_lexer **current, t_lexer *to_del,
		char **split, char *str)
{
	free_lexer_node(current, to_del);
	free_split(split);
	free(str);
}

void	update_double_quotes_case(t_tools *tools, t_lexer *current)
{
	char	*str;

	str = string_converter(current, tools);
	if (current->str)
		free(current->str);
	current->str = ft_strdup(str);
	free(str);
}

void	update_no_quotes_case(t_tools *tools, t_lexer **current, t_lexer **tmp)
{
	char	*str;
	char	**split;
	int		i;
	t_lexer	*to_del;

	str = string_converter(*tmp, tools);
	split = ft_split(str, " ");
	to_del = *tmp;
	(*tmp) = (*tmp)->next;
	if (!split || !*split)
		return (handle_empty_split(current, to_del, &split, &str), (void)0);
	i = 0;
	while (split[i])
		i++;
	if (i == 1)
		return (handle_simple_string(to_del, &split, &str), (void)0);
	insert_split_tokens(current, to_del, split, i);
	free_update_resources(current, to_del, split, str);
}

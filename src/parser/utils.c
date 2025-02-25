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

t_parser	*create_new_parser_node(void)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->tokens = NULL;
	parser->builtin = NULL;
	parser->redirects = NULL;
	parser->hd_delimiters = NULL;
	parser->next = NULL;
	parser->prev = NULL;
	return (parser);
}

void	parser_add_back(t_parser **parser)
{
	t_parser	*new_node;
	t_parser	*last_node;

	new_node = create_new_parser_node();
	if (!new_node)
		return ;
	if (*parser == NULL)
	{
		*parser = new_node;
		return ;
	}
	last_node = *parser;
	while (last_node->next != NULL)
		last_node = last_node->next;
	last_node->next = new_node;
	new_node->prev = last_node;
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
	while (split[i + 1])
		i++;
	if (i == 0)
		return (handle_simple_string(to_del, &split, &str), (void)0);
	while (i >= 0)
	{
		if (!split[i + 1])
			insert_at(current, to_del, split[i], to_del->space);
		else
			insert_at(current, to_del, split[i], 1);
		i--;
	}
	(free_lexer_node(current, to_del), free_split(split), free(str));
}

char	*string_converter(t_lexer *current, t_tools *tools)
{
	char	*result;
	int		i;
	int		j;
	size_t	size;

	size = get_new_string_length(current, tools) + 1;
	result = ft_calloc(size, 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (current->str[i])
	{
		if (current->quote_type == DOUBLE_QUOTES && current->str[i] == '\\')
		{
			if (current->str[i + 1] && (current->str[
						i + 1] == '"' || current->str[
						i + 1] == '$' || current->str[i + 1] == '\\'))
			{
				result[j++] = current->str[i + 1];
				i += 2;
				continue ;
			}
			else
			{
				result[j++] = current->str[i++];
				continue ;
			}
		}
		if (current->str[i] == '$')
		{
			handle_dollar_sign_str(current, tools, &i, &result);
			j = ft_strlen(result);
			continue ;
		}
		result[j++] = current->str[i++];
	}
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:59:23 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 17:59:23 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	process_escape_in_double_quotes(t_lexer *current, t_conv *conv)
{
	if (current->str[conv->i + 1] && (current->str[
				conv->i + 1] == '"' || current->str[conv->i + 1
			] == '$' || current->str[conv->i + 1] == '\\'))
	{
		conv->result[conv->j++] = current->str[conv->i + 1];
		conv->i += 2;
	}
	else
	{
		conv->result[conv->j++] = current->str[conv->i];
		conv->i++;
	}
}

static void	process_dollar(t_lexer *current, t_tools *tools, t_conv *conv)
{
	handle_dollar_sign_str(current, tools, &conv->i, &conv->result);
	conv->j = ft_strlen(conv->result);
}

static void	process_char(t_lexer *current, t_tools *tools, t_conv *conv)
{
	if (current->quote_type == DOUBLE_QUOTES && current->str[conv->i] == '\\')
		process_escape_in_double_quotes(current, conv);
	else if (current->str[conv->i] == '$')
		process_dollar(current, tools, conv);
	else
		conv->result[conv->j++] = current->str[conv->i++];
}

char	*string_converter(t_lexer *current, t_tools *tools)
{
	t_conv	conv;
	size_t	size;

	size = get_new_string_length(current, tools) + 1;
	conv.result = ft_calloc(size, 1);
	if (!conv.result)
		return (NULL);
	conv.i = 0;
	conv.j = 0;
	while (current->str[conv.i])
		process_char(current, tools, &conv);
	return (conv.result);
}

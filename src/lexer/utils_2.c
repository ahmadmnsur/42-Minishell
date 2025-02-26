/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 16:34:21 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 16:34:21 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*remove_escaped_quotes(const char *str)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] == '"')
		{
			result[j++] = '"';
			i += 2;
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}

// 3. Add a new handler for semicolons
// void	handle_semicolon(t_lexer **lexer, int *i)
// {
// 	add_lexer_and_increment_i(lexer, TOKEN_SEMICOLON, i, 1);
// }

int	get_nb_of_pipes(t_lexer *lexer)
{
	int		nb_pipes;
	t_lexer	*tmp;

	nb_pipes = 0;
	if (!lexer || lexer->token == TOKEN_EOF)
		return (0);
	tmp = lexer;
	while (tmp)
	{
		if (tmp->token == TOKEN_PIPE)
			nb_pipes++;
		tmp = tmp->next;
	}
	return (nb_pipes);
}

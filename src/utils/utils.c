/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:52:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:52:50 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	print_heredoc_error(char *delimiter)
{
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putendl_fd("')", 2);
}

void	free_tools(t_tools *tools)
{
	free_lexer(&(tools->lexer));
	free_parser_list(&(tools->parser));
	tools = NULL;
}

int	ft_isspacee(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
		if (str[i] != ' ')
			return (0);
	return (1);
}

int	is_a_special_case(t_lexer *current, int *i)
{
	return ((current->str[*i] && ft_isdigit(current->str[*i]))
		|| (current->str[*i] && current->str[*i] == '$'));
}

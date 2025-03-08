/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 19:29:41 by mjamil            #+#    #+#             */
/*   Updated: 2025/03/02 19:29:41 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	init_parser_node(t_parser **parser, t_lexer *lexer, const char *str)
{
	int	command_count;
	int	i;
	int	j;

	if (!str || !str[0])
		return ;
	j = 0;
	while (str[j])
	{
		if (str[j] != ' ' && (str[j] < 9 || str[j] > 13))
			break ;
		j++;
	}
	if (str[j] == '\0')
		return ;
	command_count = get_nb_of_pipes(lexer);
	i = 0;
	while (i <= command_count)
	{
		parser_add_back(parser);
		i++;
	}
}

// int	get_nb_of_semicolon(t_lexer *lexer)
// {
// 	int		count;
// 	t_lexer	*current;

// 	current = lexer;
// 	count = 0;
// 	while (current)
// 	{
// 		if (current->token == TOKEN_SEMICOLON)
// 			count++;
// 		current = current->next;
// 	}
// 	return (count);
// }
int	check_exclamation(char *prompt, t_tools *tools)
{
	if (prompt[0] == '!' && prompt[1] == '\0')
	{
		tools->last_exit_status = 2;
		return (free_tools(tools), 0);
	}
	return (1);
}

int	check_colon(char *prompt, t_tools *tools)
{
	if (prompt[0] == ':' && prompt[1] == '\0')
	{
		tools->last_exit_status = 0;
		return (free_tools(tools), 0);
	}
	return (1);
}

int	is_directory_path(char *str)
{
	int	i;
	int	j;

	if (!str || str[0] != '/')
		return (0);
	i = 0;
	while (str[i])
	{
		while (str[i] && str[i] == '/')
			i++;
		if (!str[i])
			break ;
		j = i;
		while (str[j] && str[j] != '/')
			j++;
		if (j - i != 1 && j - i != 2)
			return (0);
		if (j - i == 1 && str[i] != '.')
			return (0);
		if (j - i == 2 && (str[i] != '.' || str[i + 1] != '.'))
			return (0);
		i = j;
	}
	return (1);
}

int	is_only_ands(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] != '&')
			return (0);
		i++;
	}
	return (i);
}

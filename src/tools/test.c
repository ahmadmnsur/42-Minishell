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

int	init_tools(t_tools *tools, char *str, char **envp)
{
	tools->lexer = NULL;
	tools->parser = NULL;
	(void)envp;
	parse_string(&(tools->lexer), str);
	init_parser_node(&(tools->parser), tools->lexer, str);
	set_tokens_lexer(&(tools->parser), tools->lexer);
	if (!check_initial_errors(str, tools))
		return (free(str), 0);
	free(str);
	free_lexer(&(tools->lexer));
	update_parser_tokens(tools);
	init_redirections(tools->parser);
	set_hd_limiter_parser(tools->parser);
	set_builtins(tools);
	return (1);
}
void	init_parser_node(t_parser **parser, t_lexer *lexer, const char *str)
{
	int	pipe_count;
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
		return ; // If the string is only spaces, return early

	pipe_count = get_nb_of_pipes(lexer);
	i = 0;
	while (i <= pipe_count)
	{
		parser_add_back(parser);
		i++;
	}
}


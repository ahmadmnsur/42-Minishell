/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
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

	command_count = get_nb_of_pipes(lexer) + get_nb_of_semicolon(lexer);
	i = 0;
	while (i <= command_count)
	{
		parser_add_back(parser);
		i++;
	}
}

int get_nb_of_semicolon(t_lexer *lexer)
{
    int count;
    t_lexer *current;

	current = lexer;
	count = 0;
    while (current)
    {
        if (current->token == TOKEN_SEMICOLON)
            count++;
        current = current->next;
    }
    return (count);
}

int	check_initial_errors(char *prompt, t_tools *tools)
{
	char	c;

	if (check_unclosed_quotes(prompt))
	{
		c = check_unclosed_quotes(prompt);
		if (c == '\'')
			print_syntax_error("'", &(tools->last_exit_status));
		else
			print_syntax_error("\"", &(tools->last_exit_status));
		return (free_tools(tools), 0);
	}
	else if (!validate_pipes_and_words(tools->parser))
	{
		print_syntax_error("|", &(tools->last_exit_status));
		return (free_tools(tools), 0);
	}
	else if (!check_parser_redirections(tools->parser))
	{
		tools->last_exit_status = 2;
		return (free_tools(tools), 0);
	}
	return (1);
}

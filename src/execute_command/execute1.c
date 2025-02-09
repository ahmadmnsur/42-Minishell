/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:00:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/04 16:13:45 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int get_pipe_len(t_parser *parser)
{
    int i = 0;
    while (parser)  
    {
        i++;
        parser = parser->next;
    }
    return i;
}

int	execute_command(t_tools *tools, char **envp)
{
	t_parser	*parser;
	int			num_pipes;
	int			status;

	parser = tools->parser;
	num_pipes = get_parser_len(parser);
	if (num_pipes == 0)
		return (0);
	num_pipes = num_pipes - 1;
	if (num_pipes == 0)
	{
        set_execution_signals();
		status = if_no_pipe(tools, parser, envp);
        set_signals();
		return (status);
	}
    set_execution_signals();
	status = if_mult_pipe(tools, parser, num_pipes, envp);
    set_signals();
	return (status);
}
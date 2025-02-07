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
	int			nbr_of_pipe;
	int			status;

	parser = tools->parser;
	nbr_of_pipe = get_parser_len(parser);
	if (nbr_of_pipe == 0)
		return (0);
	nbr_of_pipe = nbr_of_pipe - 1;
	if (nbr_of_pipe == 0)
	{
		set_execution_signals();
		status = if_no_pipe(tools, parser, envp);
		set_signals();
		return (status);
	}
	set_execution_signals();
	status = if_mult_pipe(tools, parser, nbr_of_pipe, envp);
	set_signals();
	return (status);
}
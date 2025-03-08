/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:33:04 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 14:33:04 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*minishell_loop(int status)
{
	(void)status;
	return (readline("minishell >> "));
}

int	check_pipe_error(t_tools *tools)
{
	if (!validate_pipes_and_words(tools->parser))
	{
		print_syntax_error("|", &(tools->last_exit_status));
		free_tools(tools);
		return (0);
	}
	return (1);
}

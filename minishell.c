/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:02:49 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 16:30:25 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fcntl.h"
#include "unistd.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*prompt;
	char	*cmd;
	t_tools	tools;

	art();
	set_init(argc, argv, &(tools.last_exit_status), &(tools.env));
	(copy_envp(&(tools.env), envp), initialize_environment(&(tools.env)));
	while (1)
	{
		prompt = minishell_loop(tools.last_exit_status);
		set_ctrl_d(&tools, prompt);
		check_signal_if_recieved(&(tools.last_exit_status));
		if (!ft_isspacee(prompt))
			add_history(prompt);
		cmd = ft_strdup(prompt);
		free(prompt);
		if (init_tools(&tools, cmd, envp))
			tools.last_exit_status = execute_command(&tools, envp);
		free_parser_list(&(tools.parser));
	}
	(free_env(&(tools.env)), rl_clear_history());
	return (0);
}

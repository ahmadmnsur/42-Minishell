/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe32.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 22:34:01 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/16 22:34:01 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Close all pipes
void	close_all_pipes(int nbr_of_pipe, int **pipes)
{
	int	i;

	i = 0;
	while (i < nbr_of_pipe)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

// Free all pipes
void	free_pipes(int nbr_of_pipe, int **pipes)
{
	int	i;

	i = 0;
	while (i < nbr_of_pipe)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

// Count the number of commands in the parser
int	count_commands(t_parser *parser)
{
	int	count;

	count = 0;
	while (parser)
	{
		count++;
		parser = parser->next;
	}
	return (count);
}

void	error_command_not_found(const char *cmd)
{
	write(2, "minishell: ", 11);
	write(2, cmd, strlen(cmd));
	write(2, ": command not found\n", 21);
	exit(127);
}

void	exec_child_command_exec(t_parser *curr, t_tools *tools)
{
	int		status;
	char	*path;
	char	**args;
	char	**local_envp;

	if (curr->redirects && process_redirections_child
		(tools, curr->redirects) != 0)
		exit(EXIT_FAILURE);
	if (curr->builtin)
	{
		status = curr->builtin(curr, &tools->env, tools);
		exit(status);
	}
	if (!curr->tokens || !curr->tokens->str)
		exit(0);
	path = get_command_path(curr->tokens->str, tools->env);
	if (!path)
		error_command_not_found(curr->tokens->str);
	args = build_args(curr->tokens);
	if (!args)
		(free(path), exit(EXIT_FAILURE));
	local_envp = copy_envp_to_execve(tools->env);
	(execve(path, args, local_envp), perror("minishell"), free(path));
	(free_args(args), exit(EXIT_FAILURE));
}

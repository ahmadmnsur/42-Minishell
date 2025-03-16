/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:50:51 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:50:51 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Fork all commands in the pipeline
int	fork_all_commands(t_tools *tools,
	t_parser *parser, int **pipes, pid_t *child_pids)
{
	int			i;
	int			num_cmd;
	t_parser	*curr;

	num_cmd = count_commands(parser);
	i = 0;
	curr = parser;
	while (curr)
	{
		child_pids[i] = fork();
		if (child_pids[i] == -1)
			return (-1);
		else if (child_pids[i] == 0)
		{
			signal(SIGQUIT, SIG_DFL);
			setup_child_pipes(i, num_cmd, pipes);
			exec_child_command_exec(curr, tools);
		}
		else if (num_cmd > 1)
			close_parent_pipe_ends(i, num_cmd, pipes);
		i++;
		curr = curr->next;
	}
	return (0);
}

// Close unused pipe ends in the parent process
void	close_parent_pipe_ends(int i, int num_cmd, int **pipes)
{
	if (i > 0)
		close(pipes[i - 1][0]);
	if (i < num_cmd - 1)
		close(pipes[i][1]);
}

// Allocate memory for pipes and child PIDs
int	setup_mult_pipe(int num_cmd,
	int nbr_of_pipe, pid_t **child_pids, int ***pipes)
{
	*child_pids = malloc(num_cmd * sizeof(pid_t));
	if (!*child_pids)
		return (-1);
	if (nbr_of_pipe > 0)
	{
		*pipes = alloc_pipes(nbr_of_pipe);
		if (!*pipes)
		{
			free(*child_pids);
			return (-1);
		}
	}
	else
		*pipes = NULL;
	return (0);
}

// Wait for all child processes to finish
int	wait_for_all_children(int num_cmd, pid_t *child_pids)
{
	int		i;
	int		status;
	int		last_status;
	pid_t	wpid;

	i = 0;
	last_status = 0;
	reset_signals_ok();
	while (i < num_cmd)
	{
		wpid = waitpid(child_pids[i], &status, 0);
		if (wpid == child_pids[num_cmd - 1])
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}

// Main function for executing multiple piped commands
int	if_mult_pipe(t_tools *tools, t_parser *parser, int num_pipes)
{
	const int	num_cmd = num_pipes + 1;
	pid_t		*child_pids;
	int			**pipes;
	int			last_status;

	if (process_all_heredocs(parser, tools) != 0)
		return (130);
	if (setup_mult_pipe(num_cmd, num_cmd - 1, &child_pids, &pipes) != 0)
		return (-1);
	if (fork_all_commands(tools, parser, pipes, child_pids) != 0)
		return (-1);
	if (num_cmd > 1)
		close_all_pipes(num_cmd - 1, pipes);
	last_status = wait_for_all_children(num_cmd, child_pids);
	if (num_cmd > 1)
		free_pipes(num_cmd - 1, pipes);
	free(child_pids);
	return (last_status);
}

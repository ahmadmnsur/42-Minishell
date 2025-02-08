/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:00:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/31 23:22:11 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int if_mult_pipe(t_tools *tools, t_parser *parser, int num_pipes, char **envp)
{
	
    int num_cmd = num_pipes + 1;
    int status;
    int last_status = 0;
    pid_t *child_pids = malloc(num_cmd * sizeof(pid_t));
    if (!child_pids)
        return (-1);
    int nbr_of_pipe = num_cmd - 1;
    int **pipes = NULL;
    if (nbr_of_pipe > 0) {
        pipes = malloc(sizeof(int *) * nbr_of_pipe);
        if (!pipes) {
            free(child_pids);
            return (-1);
        }
        for (int i = 0; i < nbr_of_pipe; i++) {
            pipes[i] = malloc(sizeof(int) * 2);
            if (pipe(pipes[i]) < 0) {
                perror("minishell: pipe");
                for (int j = 0; j < i; j++) {
                    free(pipes[j]);
                }
                free(pipes);
                free(child_pids);
                return (-1);
            }
        }
    }

    int i = 0;
    t_parser *curr = parser;
    while (curr)
    {
        child_pids[i] = fork();
        if (child_pids[i] == -1)
        {
            perror("minishell: fork");
            return (-1);
        }
        else if (child_pids[i] == 0)
        {
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < num_cmd - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            if (nbr_of_pipe > 0) {
                for (int x = 0; x < nbr_of_pipe; x++) {
                    close(pipes[x][0]);
                    close(pipes[x][1]);
                }
            }
			if (parser->redirects)
    			process_redirections_child(tools, parser->redirects);
            if_no_pipe(tools, curr, envp);
            exit(EXIT_FAILURE);
        }
        else
        {
            if (i > 0)
                close(pipes[i - 1][0]);
            if (i < num_cmd - 1)
                close(pipes[i][1]);
        }
        curr = curr->next;
        i++;
    }

    if (nbr_of_pipe > 0)
    {
        for (i = 0; i < nbr_of_pipe; i++)
            close(pipes[i][0]);
    }

    for (int i = 0; i < num_cmd; i++)
	{
		pid_t wpid = waitpid(child_pids[i], &status, 0);
		if (wpid == child_pids[num_cmd - 1])
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status) - 1;
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
	}
    if (nbr_of_pipe > 0)
    {
        for (i = 0; i < nbr_of_pipe; i++)
            free(pipes[i]);
        free(pipes);
    }
    free(child_pids);
    return last_status;
}
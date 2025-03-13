/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe31.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 22:33:01 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/16 22:33:01 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	open_file_by_token(const char *filename, int token)
{
	if (token == TOKEN_REDIRECT_IN)
		return (open(filename, O_RDONLY));
	else if (token == TOKEN_REDIRECT_OUT)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (token == TOKEN_APPEND)
		return (open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
	printf("minishell: syntax error near unexpected token\n");
	return (-1);
}

// Get the file descriptor for redirection
int	get_redirection_fd(t_lexer *current)
{
	char	*filename;
	int		fd;

	if (!current->next || current->next->token != TOKEN_WORD)
	{
		printf("minishell: syntax error near unexpected token\n");
		return (-1);
	}
	filename = current->next->str;
	if (!filename)
	{
		printf("minishell: error processing redirection\n");
		return (-1);
	}
	fd = open_file_by_token(filename, current->token);
	if (fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	return (fd);
}

// Process redirections in the child process
int	process_redirections_child(t_tools *tools, t_lexer *redirects)
{
	t_lexer	*current;
	int		fd;

	(void)tools;
	current = redirects;
	while (current)
	{
		fd = get_redirection_fd(current);
		if (fd == -1)
			return (-1);
		if (dup_fd_for_token(fd, current->token) == -1)
		{
			close(fd);
			return (-1);
		}
		close(fd);
		current = current->next->next;
	}
	return (0);
}

// Allocate pipes for the pipeline
int	**alloc_pipes(int nbr_of_pipe)
{
	int		**pipes;
	int		i;

	pipes = malloc(sizeof(int *) * nbr_of_pipe);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < nbr_of_pipe)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) < 0)
		{
			perror("minishell: pipe");
			while (i-- > 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

// Set up pipes for the child process
void	setup_child_pipes(int i, int num_cmd, int **pipes)
{
	if (i > 0)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
	}
	if (i < num_cmd - 1)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
	}
	close_all_pipes(num_cmd - 1, pipes);
}

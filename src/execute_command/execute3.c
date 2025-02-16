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

int	process_all_heredocs(t_parser *parser, t_tools *tools)
{
	t_parser	*curr;
	t_lexer		*redirect;
	char		*delimiter;
	char		*filename ;

	curr = parser;
	while (curr)
	{
		redirect = curr->redirects;
		while (redirect)
		{
			if (redirect->token == TOKEN_HEREDOC)
			{
				if (!redirect->next || redirect->next->token != TOKEN_WORD)
				{
					fprintf(stderr,
						"minishell: syntax error near unexpected token\n");
					return (-1);
				}
				delimiter = redirect->next->str;
				filename = handle_heredoc_case
					(&delimiter, tools, redirect->quote_type);
				if (!filename && g_signum == SIGINT)
				{
					g_signum = 0;
					return (130);
				}
				if (!filename)
					return (-1);
				redirect->token = TOKEN_REDIRECT_IN;
				free(redirect->next->str);
				redirect->next->str = filename;
			}
			redirect = redirect->next;
		}
		curr = curr->next;
	}
	return (0);
}

int	process_redirections_child(t_tools *tools, t_lexer *redirects)
{
	t_lexer	*current;
	int		fd;
	char	*filename;

	(void)tools;
	current = redirects;
	while (current)
	{
		if (!current->next || current->next->token != TOKEN_WORD)
		{
			fprintf(stderr, "minishell: syntax error near unexpected token\n");
			return (-1);
		}
		filename = current->next->str;
		if (!filename)
		{
			fprintf(stderr, "minishell: error processing redirection\n");
			return (-1);
		}
		if (current->token == TOKEN_REDIRECT_IN)
			fd = open(filename, O_RDONLY);
		else if (current->token == TOKEN_REDIRECT_OUT)
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (current->token == TOKEN_APPEND)
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
		{
			fprintf(stderr, "minishell: syntax error near unexpected token\n");
			return (-1);
		}
		if (fd == -1)
		{
			perror("minishell");
			return (-1);
		}
		if (current->token == TOKEN_REDIRECT_IN)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("minishell");
				close(fd);
				return (-1);
			}
		}
		else
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("minishell");
				close(fd);
				return (-1);
			}
		}
		close(fd);
		current = current->next->next;
	}
	return (0);
}

int	if_mult_pipe(t_tools *tools, t_parser *parser, int num_pipes, char **envp)
{
	int			num_cmd;
	int			status;
	int			last_status;
	pid_t		*child_pids;
	int			nbr_of_pipe;
	int			**pipes;
	t_parser	*curr;
	char		*path;
	char		**args;
	pid_t		wpid;
	int			i;
	int			j;
	int			x;

	j = 0;
	i = 0;
	last_status = 0;
	if (process_all_heredocs(parser, tools) != 0)
		return (130);
	num_cmd = num_pipes + 1;
	last_status = 0;
	child_pids = malloc(num_cmd * sizeof(pid_t));
	if (!child_pids)
		return (-1);
	nbr_of_pipe = num_cmd - 1;
	pipes = NULL;
	if (nbr_of_pipe > 0)
	{
		pipes = malloc(sizeof(int *) * nbr_of_pipe);
		if (!pipes)
		{
			free(child_pids);
			return (-1);
		}
		while (i < nbr_of_pipe)
		{
			pipes[i] = malloc(sizeof(int) * 2);
			if (pipe(pipes[i]) < 0)
			{
				j = 0;
				perror("minishell: pipe");
				while (j < i)
					free(pipes[j]);
				free(pipes);
				free(child_pids);
				return (-1);
				j++;
			}
			i++;
		}
	}
	i = 0;
	curr = parser;
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
			if (nbr_of_pipe > 0)
			{
				x = 0;
				while (x < nbr_of_pipe)
				{
					close(pipes[x][0]);
					close(pipes[x][1]);
					x++;
				}
			}
			if (curr->redirects)
			{
				if (process_redirections_child(tools, curr->redirects) != 0)
					exit(EXIT_FAILURE);
			}
			if (!curr->tokens || !curr->tokens->str)
				exit(0);
			path = get_command_path(curr->tokens->str, tools->env);
			if (!path)
			{
				fprintf(stderr,
					"minishell: %s: command not found\n", curr->tokens->str);
				exit(127);
			}
			args = build_args(curr->tokens);
			if (!args)
			{
				free(path);
				exit(EXIT_FAILURE);
			}
			execve(path, args, envp);
			perror("minishell");
			free(path);
			j = 0;
			while (args[j])
			{
				free(args[j]);
				j++;
			}
			free(args);
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
		i = 0;
		while (i < nbr_of_pipe)
		{
			close(pipes[i][0]);
			i++;
		}
	}
	i = 0;
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
	if (nbr_of_pipe > 0)
	{
		i = 0;
		while (i < nbr_of_pipe)
		{
			free(pipes[i]);
			i++;
		}
		free(pipes);
	}
	free(child_pids);
	return (last_status);
}

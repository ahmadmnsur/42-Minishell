/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:50:42 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:50:42 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/* Gets the command path, builds args, and calls execve.
   If execve fails, prints an error and frees resources. */
void	execute_external_command(t_tools *tools, t_parser *parser)
{
	char	*path;
	char	**args;
	char	**local_envp;

	path = get_command_path(parser->tokens->str, tools->env);
	if (!path)
	{
		printf("minishell: %s: command not found\n",
			parser->tokens->str);
		exit(127);
	}
	args = build_args(parser->tokens);
	if (!args)
	{
		free(path);
		exit(EXIT_FAILURE);
	}
	local_envp = copy_envp_to_execve(tools->env);
	execve(path, args, local_envp);
	perror("minishell");
	free(path);
	free_args(args);
	exit(EXIT_FAILURE);
}

/* Validates tokens: if the first token is missing or empty,
   it advances until a TOKEN_WORD is found. */
void	validate_and_update_tokens(t_parser *parser)
{
	t_lexer	*tmp;

	if (!parser->tokens || !parser->tokens->str)
	{
		tmp = parser->tokens;
		while (tmp && tmp->token != TOKEN_WORD)
			tmp = tmp->next;
		if (!tmp || !tmp->str)
			exit(0);
		parser->tokens = tmp;
	}
}

/* Called in the child process to apply redirections, validate tokens,
   and execute the external command. */
void	child_process_no_pipe(t_tools *tools, t_parser *parser)
{
	signal(SIGQUIT, SIG_DFL);
	if (parser->redirects && process_redirections_child_for_no_pipe
		(tools, parser->redirects) != 0)
		exit(EXIT_FAILURE);
	validate_and_update_tokens(parser);
	execute_external_command(tools, parser);
}

/* Waits for the child process and returns its exit status. */
int	wait_for_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

/* Main function handling the no-pipe case:
   - Processes heredocs.
   - If a builtin is present, executes it in-process.
   - Otherwise forks and runs the external command. */
int	if_no_pipe(t_tools *tools, t_parser *parser, char **envp)
{
	int		exit_status;
	pid_t	pid;

	(void)envp;
	if (process_all_heredocs(parser, tools) != 0)
		return (130);
	if (parser->builtin)
		return (execute_builtin_no_pipe(tools, parser));
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("minishell: fork");
			return (1);
		}
		else if (pid == 0)
			child_process_no_pipe(tools, parser);
		else
		{
			reset_signals_ok();
			exit_status = wait_for_child(pid);
		}
	}
	return (exit_status);
}

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

static void	initialize_shell(int argc, char **argv, char **envp, t_tools *tools)
{
	art();
	set_init(argc, argv, &tools->last_exit_status, &tools->env);
	copy_envp(&tools->env, envp);
	initialize_environment(&tools->env);
}

static void	cleanup_shell(t_tools *tools)
{
	free_env(&tools->env);
	rl_clear_history();
}

static int	prepare_and_execute(t_tools *tools, char *cmd, char **envp)
{
	(void)envp;
	tools->lexer = NULL;
	tools->parser = NULL;
	parse_string(&tools->lexer, cmd);
	init_parser_node(&tools->parser, tools->lexer, cmd);
	set_tokens_lexer(&tools->parser, tools->lexer);
	if (!check_initial_errors(cmd, tools))
		return (free(cmd), 0);
	free(cmd);
	free_lexer(&tools->lexer);
	update_parser_tokens(tools);
	init_redirections(tools->parser);
	set_hd_limiter_parser(tools->parser);
	set_builtins(tools);
	return (1);
}

static void	process_input(t_tools *tools, char **envp)
{
	char	*input;
	char	*cmd;

	input = minishell_loop(tools->last_exit_status);
	if (!input)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		cleanup_shell(tools);
		exit(tools->last_exit_status);
	}
	check_signal_if_recieved(&tools->last_exit_status);
	if (!ft_isspacee(input))
		add_history(input);
	cmd = ft_strdup(input);
	free(input);
	if (prepare_and_execute(tools, cmd, envp))
		tools->last_exit_status = execute_command(tools, envp);
	free_parser_list(&tools->parser);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_tools	tools;

	initialize_shell(argc, argv, envp, &tools);
	while (1)
	{
		process_input(&tools, envp);
	}
	cleanup_shell(&tools);
	return (0);
}

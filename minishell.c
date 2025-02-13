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

	set_init(argc, argv, &(tools.last_exit_status), &(tools.env));
	(copy_envp(&(tools.env), envp), initialize_environment(&(tools.env)));
	while (1)
	{
		prompt = minishell_loop(tools.last_exit_status);
		set_ctrl_d(&tools, prompt);
		check_signal_if_recieved(&(tools.last_exit_status));
		if (!ft_isspace(prompt))
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

void	set_init(int argc, char *argv[], int *status, t_env **env)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("minishell: Too many arguments", STDERR_FILENO);
		exit(1);
	}
	*status = 0;
	*env = NULL;
	g_signum = 0;
	set_signals();
}

char	*minishell_loop(int status)
{
	char	*prompt;
	char	*last_status;
	char	*readline_;

	if (status == 0)
		return (readline("\033[32mminishell\033[0m\033[34m[0]\033[0m$ "));
	else
	{
		prompt = ft_strdup("\033[32mminishell\033[0m");
		prompt = ft_strjoin(prompt, "\033[31m");
		prompt = ft_strjoin(prompt, "[");
		last_status = ft_itoa(status);
		prompt = ft_strjoin(prompt, last_status);
		free(last_status);
		prompt = ft_strjoin(prompt, "]\033[0m$ ");
		readline_ = readline(prompt);
		free(prompt);
	}
	return (readline_);
}

void	print_full_env(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return ;
	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			printf("export %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("export %s\n", tmp->key);
		tmp = tmp->next;
	}
}

void	set_env_var(t_env **env, char *key, char *value, int hidden)
{
	t_env	*to_get;
	char	*new_val;

	if (hidden && get_env_var(*env, key))
		return ;
	if (!*env)
	{
		env_add_back(env, key, value, hidden);
		return ;
	}
	to_get = get_env_var(*env, key);
	if (!to_get)
		env_add_back(env, key, value, hidden);
	else
	{
		new_val = ft_strdup(value);
		free(to_get->value);
		to_get->value = new_val;
		to_get->hidden = hidden;
	}
}

void	free_simple_node(t_env **env, t_env *to_del)
{
	t_env	*tmp;

	if (!env || !*env || !to_del)
		return ;
	if (*env == to_del)
	{
		*env = (*env)->next;
		if (to_del->key)
			free(to_del->key);
		if (to_del->value)
			free(to_del->value);
		free(to_del);
		to_del = NULL;
		return ;
	}
	tmp = *env;
	while (tmp->next != to_del)
		tmp = tmp->next;
	tmp->next = to_del->next;
	if (to_del->key)
		free(to_del->key);
	if (to_del->value)
		free(to_del->value);
	free(to_del);
	to_del = NULL;
}

void	unset_env_var(t_env **env, char *key)
{
	t_env	*to_del;

	if (!env || !*env || !key)
		return ;
	to_del = get_env_var(*env, key);
	if (!to_del)
		return ;
	free_simple_node(env, to_del);
}

void	copy_envp(t_env **env, char **envp)
{
	int		i;
	char	**split;

	if (!envp || !*envp)
		return ;
	i = 0;
	while (envp[i])
	{
		split = ft_split(envp[i], "=");
		set_env_var(env, split[0], split[1], 0);
		free_split(split);
		split = NULL;
		i++;
	}
	free_split(split);
	update_shlvl(env, 0);
}

void	update_shlvl(t_env **env, int hidden)
{
	char	*shlvl_str;
	int		shlvl;

	shlvl_str = get_env_var(*env, "SHLVL")->value;
	shlvl = ft_atoi(shlvl_str);
	shlvl++;
	shlvl_str = ft_itoa(shlvl);
	set_env_var(env, "SHLVL", shlvl_str, hidden);
	free(shlvl_str);
}
void	initialize_environment(t_env **env)
{
	char	cwd[1024];

	if (getenv("PWD"))
		return ;
	if (getcwd(cwd, sizeof(cwd)))
		set_env_var(env, "PWD", cwd, 0);
	else
		perror("minishell: getcwd");
	set_env_var(env, "OLDPWD", NULL, 1);
	set_env_var(env, "SHLVL", "1", 0);
}
int	init_tools(t_tools *tools, char *str, char **envp)
{
	tools->lexer = NULL;
	tools->parser = NULL;
	(void)envp;
	parse_string(&(tools->lexer), str);
	init_parser_nodes(&(tools->parser), tools->lexer, str);
	set_tokens_lexer(&(tools->parser), tools->lexer);
	if (!check_initial_errors(str, tools))
		return (free(str), 0);
	free(str);
	free_lexer(&(tools->lexer));
	update_parser_tokens(tools);
	init_redirections(tools->parser);
	set_hd_limiter_parser(tools->parser);
	set_builtins(tools);
	return (1);
}

void	free_tools(t_tools *tools)
{
	free_lexer(&(tools->lexer));
	free_parser_list(&(tools->parser));
	tools = NULL;
}

int	check_initial_errors(char *prompt, t_tools *tools)
{
	char	c;

	if (check_unclosed_quotes(prompt))
	{
		c = check_unclosed_quotes(prompt);
		if (c == '\'')
			print_syntax_error("'", &(tools->last_exit_status));
		else
			print_syntax_error("\"", &(tools->last_exit_status));
		return (free_tools(tools), 0);
	}
	else if (!check_pipes(tools->parser))
	{
		print_syntax_error("|", &(tools->last_exit_status));
		return (free_tools(tools), 0);
	}
	else if (!check_parser_redirections(tools->parser))
	{
		tools->last_exit_status = 2;
		return (free_tools(tools), 0);
	}
	return (1);
}

void	print_message_error(char *s1, char *s2, char *s3, char *s4)
{
	ft_putstr_fd("minishell: ", 2);
	if (s1)
		ft_putstr_fd(s1, 2);
	if (s2)
		ft_putstr_fd(s2, 2);
	if (s3)
		ft_putstr_fd(s3, 2);
	if (s4)
		ft_putstr_fd(s4, 2);
	ft_putstr_fd("\n", 2);
}
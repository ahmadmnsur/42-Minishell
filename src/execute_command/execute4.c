/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:50:59 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:50:59 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*expand_heredoc_line(char *line, t_env *env, int last_ret, int expand_variables)
{
	char	*expanded;
	size_t	i = 0, j = 0, k, key_start;
	char	ret_str[12];
	char	*key;
	t_env	*env_var;

	expanded = malloc(1000);
	if (!expanded)
		return (NULL);
	while (line[j])
	{
		if (expand_variables && line[j] == '$' && (line[j + 1] == '?' || ft_isalpha((unsigned char)line[j + 1]) || line[j + 1] == '_'))
		{
			j++;
			if (line[j] == '?')
			{
				snprintf(ret_str, sizeof(ret_str), "%d", last_ret);
				k = 0;
				while (ret_str[k])
					expanded[i++] = ret_str[k++];
				j++;
			}
			else
			{
				key_start = j;
				while (ft_isalnum((unsigned char)line[j]) || line[j] == '_')
					j++;
				key = ft_strndupp(line + key_start, j - key_start);
				if (!key)
					continue;
				env_var = get_env_var(env, key);
				free(key);
				if (env_var && env_var->value)
				{
					k = 0;
					while (env_var->value[k])
						expanded[i++] = env_var->value[k++];
				}
			}
		}
		else
			expanded[i++] = line[j++];
	}
	expanded[i] = '\0';
	return (expanded);
}

char	*handle_heredoc_case(char **delimiters, t_tools *tools, t_quote_type quote_type)
{
	char	*delimiter;
	char	template[] = "/tmp/minishell_heredocXXXXXX";
	int		tmp_fd, stdin_copy;
	char	*line, *expanded_line;
	int		expand_variables;

	stdin_copy = dup(STDIN_FILENO);
	if (stdin_copy == -1)
	{
		perror("minishell");
		return (NULL);
	}
	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
	g_signum = 0;
	if (!delimiters || !delimiters[0])
	{
		close(stdin_copy);
		reset_signals();
		return (NULL);
	}
	delimiter = ft_strtrim(delimiters[0], " \t\n");
	if (!delimiter)
	{
		perror("minishell");
		close(stdin_copy);
		reset_signals();
		return (NULL);
	}
	tmp_fd = mkstemp(template);
	if (tmp_fd == -1)
	{
		perror("minishell");
		free(delimiter);
		close(stdin_copy);
		reset_signals();
		return (NULL);
	}
	expand_variables = (quote_type == NO_QUOTE);
	while (1)
	{
		line = readline("> ");
		if (g_signum == SIGINT)
		{
			free(line);
			close(tmp_fd);
			unlink(template);
			free(delimiter);
			dup2(stdin_copy, STDIN_FILENO);
			close(stdin_copy);
			reset_signals();
			return (NULL);
		}
		if (!line)
		{
			write(2, "here-document delimited by end-of-file (wanted `", 48);
			write(2, delimiter, strlen(delimiter));
			write(2, "')\n", 3);
			break;
		}
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		if (expand_variables)
		{
			expanded_line = expand_heredoc_line(line, tools->env, tools->last_exit_status, 1);
			write(tmp_fd, expanded_line, strlen(expanded_line));
			write(tmp_fd, "\n", 1);
			free(expanded_line);
		}
		else
		{
			write(tmp_fd, line, strlen(line));
			write(tmp_fd, "\n", 1);
		}
		free(line);
	}
	dup2(stdin_copy, STDIN_FILENO);
	close(stdin_copy);
	free(delimiter);
	close(tmp_fd);
	reset_signals();
	if (g_signum == SIGINT)
	{
		unlink(template);
		return (NULL);
	}
	return (ft_strdup(template));
}

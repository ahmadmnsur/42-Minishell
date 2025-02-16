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

t_env	*get_env_var(t_env *env, char *key)
{
	t_env	*tmp;

	if (!env)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (ft_strlen(tmp->key) == ft_strlen(key)
			&& ft_strncmp(tmp->key, key, ft_strlen(key)) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*expand_heredoc_line(char *line, t_env *env,
	int last_ret, int expand_variables)
{
	char	*expanded;
	size_t	i;
	size_t	j;
	size_t	k;
	char	ret_str[12];
	size_t	key_start;
	char	*key;
	t_env	*env_var;

	expanded = malloc(1000);
	if (!expanded)
		return (NULL);
	i = 0;
	j = 0;
	while (line[j])
	{
		if (expand_variables && line[j]
			== '$' && (line[j + 1] == '?' || ft_isalpha(
					(unsigned char)line[j + 1]) || line[j + 1] == '_'))
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
					continue ;
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

char	*handle_heredoc_case(char **delimiters,
	t_tools *tools, t_quote_type quote_type)
{
	char	*delimiter;
	char	template[sizeof("/tmp/minishell_heredocXXXXXX")];
	int		tmp_fd;
	FILE	*tmp_file;
	char	*line;
	char	*expanded_line;
	int		expand_variables;
	int		stdin_copy;

	stdin_copy = dup(STDIN_FILENO);
	ft_strlcpy(template, "/tmp/minishell_heredocXXXXXX", sizeof(template));
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
	tmp_file = fdopen(tmp_fd, "w+");
	if (!tmp_file)
	{
		perror("minishell");
		close(tmp_fd);
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
			fclose(tmp_file);
			unlink(template);
			free(delimiter);
			dup2(stdin_copy, STDIN_FILENO);
			close(stdin_copy);
			reset_signals();
			return (NULL);
		}
		if (!line)
		{
			fprintf(stderr,
				"here-document delimited by end-of-file (wanted `%s')\n",
				delimiter);
			break ;
		}
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand_variables)
		{
			expanded_line = expand_heredoc_line(line, tools->env,
					tools->last_exit_status, 1);
			fprintf(tmp_file, "%s\n", expanded_line);
			free(expanded_line);
		}
		else
			fprintf(tmp_file, "%s\n", line);
		free(line);
	}
	dup2(stdin_copy, STDIN_FILENO);
	close(stdin_copy);
	free(delimiter);
	fflush(tmp_file);
	fclose(tmp_file);
	reset_signals();
	if (g_signum == SIGINT)
	{
		unlink(template);
		return (NULL);
	}
	return (ft_strdup(template));
}

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

static int	heredoc_setup(t_heredoc_ctx *ctx, char **delimiters, \
			t_quote_type quote_type)
{
	ctx->stdin_copy = dup(STDIN_FILENO);
	if (ctx->stdin_copy == -1)
		return (perror("minishell"), -1);
	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
	g_signum = 0;
	if (!delimiters || !delimiters[0])
		return (close(ctx->stdin_copy), reset_signals(), -1);
	ctx->delimiter = ft_strtrim(delimiters[0], " \t\n");
	if (!ctx->delimiter)
	{
		perror("minishell");
		return (close(ctx->stdin_copy), reset_signals(), -1);
	}
	ctx->tmp_fd = mkstemp(ctx->template);
	if (ctx->tmp_fd == -1)
	{
		perror("minishell");
		free(ctx->delimiter);
		return (close(ctx->stdin_copy), reset_signals(), -1);
	}
	ctx->expand_variables = (quote_type == NO_QUOTE);
	return (0);
}

static char	*heredoc_cleanup(t_heredoc_ctx *ctx)
{
	dup2(ctx->stdin_copy, STDIN_FILENO);
	close(ctx->stdin_copy);
	free(ctx->delimiter);
	close(ctx->tmp_fd);
	reset_signals();
	if (g_signum == SIGINT)
	{
		unlink(ctx->template);
		return (NULL);
	}
	return (ft_strdup(ctx->template));
}

char	*handle_heredoc_case(char **delimiters, t_tools *tools, \
		t_quote_type quote_type)
{
	t_heredoc_ctx	ctx;
	char			*result;
	size_t			len;

	len = ft_strlen("/tmp/minishell_heredocXXXXXX") + 1;
	ft_strlcpy(ctx.template, "/tmp/minishell_heredocXXXXXX", len);
	if (heredoc_setup(&ctx, delimiters, quote_type) != 0)
		return (NULL);
	if (heredoc_loop(&ctx, tools) != 0)
		return (NULL);
	result = heredoc_cleanup(&ctx);
	return (result);
}

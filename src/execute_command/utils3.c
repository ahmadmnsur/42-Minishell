/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 23:12:29 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/16 23:12:29 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	handle_heredoc_all_heredoc(t_lexer *redirect, t_tools *tools)
{
	char	*delimiter;
	char	*filename;

	if (!redirect->next || redirect->next->token != TOKEN_WORD)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token\n");
		return (-1);
	}
	delimiter = redirect->next->str;
	filename = handle_heredoc_case(&delimiter, tools, redirect->quote_type);
	if (!filename)
	{
		if (g_signum == SIGINT)
		{
			g_signum = 0;
			return (130);
		}
		return (-1);
	}
	redirect->token = TOKEN_REDIRECT_IN;
	free(redirect->next->str);
	redirect->next->str = filename;
	return (0);
}

static int	process_redirects_all_heredoc(t_lexer *redirect, t_tools *tools)
{
	int	ret;

	while (redirect)
	{
		if (redirect->token == TOKEN_HEREDOC)
		{
			ret = handle_heredoc_all_heredoc(redirect, tools);
			if (ret)
				return (ret);
		}
		redirect = redirect->next;
	}
	return (0);
}

int	process_all_heredocs(t_parser *parser, t_tools *tools)
{
	t_parser	*curr;
	int			ret;

	curr = parser;
	while (curr)
	{
		ret = process_redirects_all_heredoc(curr->redirects, tools);
		if (ret)
			return (ret);
		curr = curr->next;
	}
	return (0);
}

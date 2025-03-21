/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:50:13 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:50:13 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_env(t_parser *parser, t_env **env, t_tools *tools)
{
	t_env	*tmp;

	(void)parser;
	(void)tools;
	if (!env)
	{
		write(2, "env: No environment variables to display\n", 41);
		return (0);
	}
	tmp = *env;
	while (tmp != NULL)
	{
		if (tmp->hidden == 0 && tmp->value != NULL)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

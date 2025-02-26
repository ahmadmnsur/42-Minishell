/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:01:13 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:01:13 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	handle_export_identifier(char *arg, t_env *env)
{
	if (!is_valid_identifier(arg))
	{
		printf("export: %s: not a valid identifier\n", arg);
		return (1);
	}
	else if (!check_existing_key(env, arg))
		add_env_var(&env, arg, NULL, 0);
	return (0);
}

static int	process_export_args(char **args, t_env **env)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			if (handle_export_assignment(args[i], env) != 0)
				ret = 1;
		}
		else
		{
			if (handle_export_identifier(args[i], *env) != 0)
				ret = 1;
		}
		i++;
	}
	return (ret);
}

int	builtin_export(t_parser *parser, t_env *env)
{
	char	**args;
	int		ret;

	args = get_cmd(parser->tokens);
	if (!args[1])
	{
		print_full_export_list(env);
		free_split_array(args);
		return (0);
	}
	ret = process_export_args(args, &env);
	free_split_array(args);
	return (ret);
}

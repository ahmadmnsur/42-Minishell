/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 21:26:38 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 21:26:38 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_existing_key(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_cmp(current->key, key) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

static int	extract_assignment_details(char *arg, char **key, char **value)
{
	char	**arg_parts;
	int		assigned;

	arg_parts = export_split(arg, "=");
	*key = ft_strtrim(arg_parts[1], " \t\n\r");
	if (arg_parts[3] && ft_strlen(arg_parts[3]) > 0)
		assigned = 1;
	else if (arg_parts[3] && ft_strlen(arg_parts[3]) == 0)
		assigned = 1;
	else
		assigned = 0;
	if (assigned && arg_parts[3])
		*value = ft_strdup(arg_parts[3]);
	else
		*value = NULL;
	free_split_array(arg_parts);
	return (assigned);
}

static int	handle_env_update(char *key, char *value, int assigned, t_env **env)
{
	if (!is_valid_identifier(key))
	{
		printf("export: %s: not a valid identifier\n", key);
		free(key);
		if (value)
			free(value);
		return (1);
	}
	if (assigned)
		update_env_var(env, key, value, 1);
	else
		add_env_var(env, key, NULL, 0);
	free(key);
	if (value)
		free(value);
	return (0);
}

static int	process_export_assignment(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	int		assigned;

	assigned = extract_assignment_details(arg, &key, &value);
	return (handle_env_update(key, value, assigned, env));
}

int	handle_export_assignment(char *arg, t_env **env)
{
	if (process_export_assignment(arg, env))
		return (1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:34:22 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 14:34:22 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

void	env_add_back(t_env **env, char *key, char *value, int hidden)
{
	t_env	*node;
	t_env	*tmp;

	node = create_env_node(key, value, hidden);
	tmp = *env;
	if (!node)
		return ;
	if (!*env)
	{
		*env = node;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
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

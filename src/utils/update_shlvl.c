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

// void	update_shlvl(t_env **env, int hidden)
// {
// 	char	*shlvl_str;
// 	int		shlvl;

// 	shlvl_str = get_env_var(*env, "SHLVL")->value;
// 	shlvl = ft_atoi(shlvl_str);
// 	shlvl++;
// 	shlvl_str = ft_itoa(shlvl);
// 	set_env_var(env, "SHLVL", shlvl_str, hidden);
// 	free(shlvl_str);
// }

void	update_shlvl(t_env **env, int hidden)
{
	t_env	*shlvl_node;
	int		shlvl;
	char	*new_shlvl;

	shlvl_node = get_env_var(*env, "SHLVL");
	if (!shlvl_node)
		shlvl = 1;
	else
		shlvl = ft_atoi(shlvl_node->value) + 1;
	new_shlvl = ft_itoa(shlvl);
	set_env_var(env, "SHLVL", new_shlvl, hidden);
	free(new_shlvl);
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
	char	*key;
	char	*value;
	char	*equal_sign;

	if (!envp || !*envp)
		return ;
	i = 0;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			key = ft_substr(envp[i], 0, equal_sign - envp[i]);
			value = ft_strdup(equal_sign + 1);
			set_env_var(env, key, value, 0);
			free(key);
			free(value);
		}
		i++;
	}
	update_shlvl(env, 0);
}

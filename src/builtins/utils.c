/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:01:25 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:01:25 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//This function compares two strings lexicographically and returns
//the difference between the first differing characters.
int	ft_cmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

//This function searches for an environment variable by its key in
//the linked list and returns its value as a newly allocated string.
char	*get_env(t_env *head, const char *key)
{
	while (head)
	{
		if (ft_cmp(head->key, key) == 0
			&& ft_strlen(key) == ft_strlen(head->key))
			return (ft_strdup(head->value));
		head = head->next;
	}
	return (ft_strdup(""));
}

// This function adds a new environment variable to the linked list.
void	add_env_var(t_env **env, const char *key,
	const char *value, int assigned)
{
	t_env	*new_var;
	t_env	*tmp;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return ;
	new_var->key = ft_strdup(key);
	if (assigned)
		new_var->value = ft_strdup(value);
	else
		new_var->value = NULL;
	new_var->assigned = assigned;
	new_var->hidden = 0;
	new_var->next = NULL;
	if (!*env)
		*env = new_var;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_var;
	}
}

//This function updates an existing environment variable's
//value in the list or adds it if it doesn't exist.
void	update_env_var(t_env **env, const char *key,
	const char *value, int assigned)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_cmp(tmp->key, key) == 0)
		{
			if (tmp->value)
				free(tmp->value);
			if (assigned)
				tmp->value = ft_strdup(value);
			else
				tmp->value = NULL;
			tmp->assigned = assigned;
			return ;
		}
		tmp = tmp->next;
	}
	add_env_var(env, key, value, assigned);
}

//This function prints the entire environment
//variable list in a format similar to declare -x key="value".
void	print_full_export_list(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return ;
	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("declare -x %s\n", tmp->key);
		tmp = tmp->next;
	}
}

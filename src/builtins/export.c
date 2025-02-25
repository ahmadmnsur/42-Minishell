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

char	**export_split(const char *str, const char *delimiter)
{
	char	**result;
	size_t	delim_len;
	char	*delim_pos;

	result = malloc(5 * sizeof(char *));
	if (!result)
		return (NULL);
	delim_len = ft_strlen(delimiter);
	delim_pos = ft_strstr(str, delimiter);
	if (!delim_pos)
	{
		result[0] = NULL;
		result[1] = ft_strdup(str);
		result[2] = NULL;
		result[3] = NULL;
		result[4] = NULL;
		return (result);
	}
	result[0] = NULL;
	result[1] = ft_strndupp(str, delim_pos - str);
	result[2] = ft_strndupp(delimiter, delim_len);
	result[3] = ft_strdup(delim_pos + delim_len);
	result[4] = NULL;
	    // Trim spaces around the key and value
		char *trimmed_key = ft_strtrim(result[1], " \t\n\r");  
		free(result[1]);
		result[1] = trimmed_key;
	
		char *trimmed_value = ft_strtrim(result[3], " \t\n\r");
		free(result[3]);
		result[3] = trimmed_value;
		return (result);
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	handle_invalid_export(char **argcomment)
{
	printf("export: `%s`: not a valid identifier\n", argcomment[1]);
	free_split_array(argcomment);
	return (1);
}

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

static int process_export_assignment(char *arg, t_env **env)
{
    char    **arg_parts;
    char    *key;
    char    *value;
    int     assigned;

    arg_parts = export_split(arg, "=");
    key = ft_strtrim(arg_parts[1], " \t\n\r");
    // Determine if an assignment was actually provided:
    if (arg_parts[3] && ft_strlen(arg_parts[3]) > 0)
        assigned = 1;
    else if (arg_parts[3] && ft_strlen(arg_parts[3]) == 0)
        assigned = 1;  // export a= means assigned empty string
    else
        assigned = 0;  // This case might not occur here, since we check for '='

    value = NULL;
    if (assigned)
        value = ft_strdup(arg_parts[3]);  // value can be empty string

    if (!is_valid_identifier(key))
    {
        printf("export: %s: not a valid identifier\n", key);
        free(key);
        free_split_array(arg_parts);
        return (1);
    }
    if (assigned)
        update_env_var(env, key, value, 1);
    else
        add_env_var(env, key, NULL, 0);
    free(key);
    if (value)
        free(value);
    free_split_array(arg_parts);
    return (0);
}

int builtin_export(t_parser *parser, t_env *env)
{
    char    **args;
    int     i;
    int     ret;

    ret = 0;
    args = get_cmd(parser->tokens);
    if (!args[1])
        return (print_full_export_list(env), 0);
    i = 1;
    while (args[i])
    {
        if (ft_strchr(args[i], '='))
        {
            if (process_export_assignment(args[i], &env))
                ret = 1;
        }
        else
        {
            if (!is_valid_identifier(args[i]))
            {
                printf("export: %s: not a valid identifier\n", args[i]);
                ret = 1;
            }
            else if (!check_existing_key(env, args[i]))
            {
                // Add the variable as exported but not assigned
                add_env_var(&env, args[i], NULL, 0);
            }
        }
        i++;
    }
    free_split_array(args);
    return (ret);
}
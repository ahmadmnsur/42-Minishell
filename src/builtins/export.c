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

// int	builtin_export(t_parser *parser, t_env *env)
// {
// 	char	**arg;
// 	char	**argcomment;
// 	int		key_exists;

// 	arg = get_cmd(parser->tokens);
// 	if (!arg[1])
// 		return (print_full_export_list(env), 0);
// 	argcomment = export_split(arg[1], "=");
// 	if (!argcomment || !argcomment[1])
// 		return (printf("export: Invalid input\n"),
// 			free_split_array(argcomment), 1);
// 	if (!is_valid_identifier(argcomment[1]))
// 		return (handle_invalid_export(argcomment));
// 	key_exists = check_existing_key(env, argcomment[1]);
// 	if (!argcomment[2] && !key_exists)
// 		add_env_var(&env, argcomment[1], NULL);
// 	else if (argcomment[2] && !argcomment[3])
// 		update_env_var(&env, argcomment[1], "");
// 	else if (argcomment[2] && argcomment[3])
// 		update_env_var(&env, argcomment[1], argcomment[3]);
// 	free_split_array(argcomment);
// 	return (0);
// }

int	builtin_export(t_parser *parser, t_env *env)
{
	char	**args;
	char	**arg_parts;
	int		i;
	int		ret;

	ret = 0;
	args = get_cmd(parser->tokens);
	if (!args[1])
		return (print_full_export_list(env), 0);
	i = 1;
	while (args[i])
	{
		arg_parts = export_split(args[i], "=");
		if (!arg_parts || !arg_parts[1])
		{
			(printf("export: Invalid input\n"), free_split_array(arg_parts));
			ret = 1;
			i++;
			continue ;
		}
		if (!is_valid_identifier(arg_parts[1]))
		{
			ret = handle_invalid_export(arg_parts);
			i++;
			continue;
		}
		if (!arg_parts[2] && (!check_existing_key(env, arg_parts[1])))
			add_env_var(&env, arg_parts[1], NULL);
		else if (arg_parts[2] && !arg_parts[3])
			update_env_var(&env, arg_parts[1], "");
		else if (arg_parts[2] && arg_parts[3])
			update_env_var(&env, arg_parts[1], arg_parts[3]);
		free_split_array(arg_parts);
		i++;
	}
	return (ret);
}

/**
 * builtin_export - Builtin command to handle export functionality.
 * @parser: Parser context.
 * @env: Environment variables.
 *
 * Return: 0 on success, 1 on failure.
 */

//  static int handle_export_argument(t_env *env, char *arg)
// {
//     char    **arg_parts;
//     int     ret;

//     ret = 0;
//     arg_parts = export_split(arg, "=");
//     if (!arg_parts || !arg_parts[1])
//     {
//         printf("export: Invalid input\n");
//         free_split_array(arg_parts);
//         return (1);
//     }
//     if (!is_valid_identifier(arg_parts[1]))
//     {
//         ret = handle_invalid_export(arg_parts);
//     }
//     else if (!arg_parts[2] && (!check_existing_key(env, arg_parts[1])))
//         add_env_var(&env, arg_parts[1], NULL);
//     else if (arg_parts[2] && !arg_parts[3])
//         update_env_var(&env, arg_parts[1], "");
//     else if (arg_parts[2] && arg_parts[3])
//         update_env_var(&env, arg_parts[1], arg_parts[3]);
//     free_split_array(arg_parts);
//     return (ret);
// }

// int builtin_export(t_parser *parser, t_env *env)
// {
//     char    **args;
//     int     i;
//     int     ret;

//     ret = 0;
//     args = get_cmd(parser->tokens);
//     if (!args[1])
//         return (print_full_export_list(env), 0);
//     i = 1;
//     while (args[i])
//     {
//         if (handle_export_argument(env, args[i]) != 0)
//             ret = 1;
//         i++;
//     }
//     return (ret);
// }



// int	builtin_export(t_parser *parser, t_env *env)
// {
// 	char	**args;
// 	char	**arg_parts;
// 	int		i;
// 	int		ret;
// 	char	*trimmed_key;

// 	ret = 0;
// 	args = get_cmd(parser->tokens);
// 	if (!args[1])
// 		return (print_full_export_list(env), 0);
// 	i = 1;
// 	while (args[i])
// 	{
// 		arg_parts = export_split(args[i], "=");
// 		if (!arg_parts || !arg_parts[1])
// 		{
// 			(printf("export: Invalid input\n"), free_split_array(arg_parts));
// 			ret = 1;
// 			i++;
// 			continue ;
// 		}
// 		// Trim whitespace from the key
// 		trimmed_key = ft_strtrim(arg_parts[1], " \t\n\r");
// 		if (!is_valid_identifier(trimmed_key))
// 		{
// 			(printf("export: `%s`: not a valid identifier\n", arg_parts[1]), free(trimmed_key));
// 			free_split_array(arg_parts);
// 			ret = 1;
// 			i++;
// 			continue;
// 		}
// 		free(trimmed_key); // No longer needed if using trimmed key elsewhere
// 		// Proceed to add/update using trimmed key if needed
// 		// Ensure you use the trimmed key when adding to the environment
// 		// Example modification:
// 		// update_env_var(&env, trimmed_key, arg_parts[3]);
// 		// [Adjust the rest of your code to use the trimmed key]
// 		// [The following code is an example and needs adjustment based on your functions]
// 		if (!arg_parts[2] && (!check_existing_key(env, arg_parts[1])))
// 			add_env_var(&env, arg_parts[1], NULL);
// 		else if (arg_parts[2] && !arg_parts[3])
// 			update_env_var(&env, arg_parts[1], "");
// 		else if (arg_parts[2] && arg_parts[3])
// 			update_env_var(&env, arg_parts[1], arg_parts[3]);
// 		free_split_array(arg_parts);
// 		i++;
// 	}
// 	return (ret);
// }
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:01:08 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:01:08 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[0] == '-')
	{
		if (!str[i + 1])
			return (0);
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_cmp2(char *a1, char *a2)
{
	int	len1;
	int	len2;

	len1 = ft_strlen(a1);
	len2 = ft_strlen(a2);
	if (len1 < len2)
		return (-1);
	else if (len1 > len2)
		return (1);
	else
		return (ft_strncmp(a1, a2, len1));
}

int	chekk(char *str)
{
	if (str[0] == '-')
	{
		if (ft_cmp2("-9223372036854775808", str) >= 0)
			return (1);
		else
			return (0);
	}
	else
	{
		if (ft_cmp2("9223372036854775807", str) >= 0)
			return (1);
		else
			return (0);
	}
}

int	print_error(char **arg)
{
	printf("exit: %s: numeric argument required\n", arg[1]);
	return (2);
}

int	builtin_exit(t_parser *parser, t_env **env, t_tools *tools)
{
	char	**arg;
	int		exit_status;

	(void)env;
	arg = get_cmd(parser->tokens);
	if (!arg || !arg[0])
		exit(0);
	if (!arg[1])
		exit(tools->last_exit_status);
	if (is_numeric(arg[1]))
	{
		if (arg[2])
			return (printf("exit: too many arguments\n"), free_args(arg), 1);
		if (!chekk(arg[1]))
		{
			printf("exit: %s: numeric argument required\n", arg[1]);
			free_args(arg);
			exit(2);
		}
		exit_status = ft_atoi(arg[1]);
	}
	else
		exit_status = print_error(arg);
	free_args(arg);
	exit(exit_status);
}

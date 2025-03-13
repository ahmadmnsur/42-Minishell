/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:01:17 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:01:17 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

char	*fetch_pwd(t_env *env_list)
{
	char	*pwd;

	pwd = get_env(env_list, "PWD");
	if (!pwd)
		printf("Error: PWD not found in environment variables.\n");
	return (pwd);
}

void	print_pwd_with_traversal(char *pwd, int nbr)
{
	int	i;

	printf("%s", pwd);
	i = 0;
	while (i < nbr)
	{
		printf("/..");
		i++;
	}
	printf("\n");
}

int	pwd_help(t_env *env_list)
{
	int		nbr;
	char	*pwd;

	if (!env_list)
		return (1);
	nbr = env_list->two_point;
	if (nbr < 0)
		nbr = 0;
	pwd = fetch_pwd(env_list);
	if (!pwd)
		return (1);
	print_pwd_with_traversal(pwd, nbr);
	free(pwd);
	return (0);
}

int	builtin_pwd(t_parser *parser, t_env *env, t_tools *tools)
{
	char	cwd[1024];

	(void)parser;
	(void)tools;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		if (!env)
		{
			printf("Error: Environment list is NULL.\n");
			return (1);
		}
		return (pwd_help(env));
	}
}

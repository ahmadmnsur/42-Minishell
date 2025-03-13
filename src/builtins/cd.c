/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:00:56 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:00:56 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	cd_home(t_env *env)
{
	char	*home;
	char	*pwd;

	home = get_env(env, "HOME");
	pwd = get_env(env, "PWD");
	if (!home)
	{
		printf("cd: HOME not set\n");
		return (1);
	}
	if (chdir(home) != 0)
	{
		perror("cd");
		free(home);
		free(pwd);
		return (1);
	}
	set_env(&env, "OLDPWD", pwd, 0);
	set_env(&env, "PWD", home, 0);
	free(home);
	free(pwd);
	return (0);
}

int	cd_oldpwd(t_env *env)
{
	char	*pwd;
	char	*oldpwd;

	pwd = get_env(env, "PWD");
	oldpwd = get_env(env, "OLDPWD");
	if (!oldpwd)
	{
		printf("cd: OLDPWD not set\n");
		return (1);
	}
	if (chdir(oldpwd) != 0)
	{
		perror("cd");
		free(pwd);
		free(oldpwd);
		return (1);
	}
	set_env(&env, "PWD", oldpwd, 0);
	set_env(&env, "OLDPWD", pwd, 0);
	printf("%s\n", oldpwd);
	free(pwd);
	free(oldpwd);
	return (0);
}

int	cd_path(const char *path, t_env *env)
{
	char	*current_pwd;
	char	*new_pwd;

	current_pwd = get_env(env, "PWD");
	if (chdir(path) != 0)
	{
		perror("cd");
		env->two_point += 1;
		free(current_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		env->two_point += 1;
		perror("cd: error getting current directory");
		free(current_pwd);
		return (1);
	}
	env->two_point = 0;
	set_env(&env, "OLDPWD", current_pwd, 0);
	set_env(&env, "PWD", new_pwd, 0);
	free(new_pwd);
	free(current_pwd);
	return (0);
}

static int	process_cd_args(char **arg, t_env *env)
{
	int	ret;

	if (!arg[0])
		return (cd_home(env));
	if (arg[1])
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	if (ft_cmp(arg[0], "~") == 0)
		ret = cd_home(env);
	else if (ft_cmp(arg[0], "-") == 0)
		ret = cd_oldpwd(env);
	else
		ret = cd_path(arg[0], env);
	return (ret);
}

int	builtin_cd(t_parser *parser, t_env *env, t_tools *tools)
{
	char	**arg;
	int		ret;

	(void)tools;
	arg = get_cmd(parser->tokens->next);
	if (!arg)
		return (cd_home(env));
	ret = process_cd_args(arg, env);
	free_split_array(arg);
	return (ret);
}

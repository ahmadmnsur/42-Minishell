/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:48:22 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 14:48:22 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	set_init(int argc, char *argv[], int *status, t_env **env)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putendl_fd("minishell: Too many arguments", STDERR_FILENO);
		exit(1);
	}
	*status = 0;
	*env = NULL;
	g_signum = 0;
	set_signals();
}

void	initialize_environment(t_env **env)
{
	char	cwd[1024];

	if (getenv("PWD"))
		return ;
	if (getcwd(cwd, sizeof(cwd)))
		set_env_var(env, "PWD", cwd, 0);
	else
		perror("minishell: getcwd");
	set_env_var(env, "OLDPWD", NULL, 1);
	set_env_var(env, "SHLVL", "1", 0);
}

void	print_message_error(char *s1, char *s2, char *s3, char *s4)
{
	ft_putstr_fd("minishell: ", 2);
	if (s1)
		ft_putstr_fd(s1, 2);
	if (s2)
		ft_putstr_fd(s2, 2);
	if (s3)
		ft_putstr_fd(s3, 2);
	if (s4)
		ft_putstr_fd(s4, 2);
	ft_putstr_fd("\n", 2);
}

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

int	builtin_pwd(t_parser *parser, t_env *env)
{
	char	cwd[1024];

	(void)parser;
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

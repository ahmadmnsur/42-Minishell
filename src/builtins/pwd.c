

#include "../../minishell.h"

int	pwd_help(t_env *env_list)
{
	int		nbr;
	char	*pwd;
	int		i;

	if (!env_list)
		return (1);
	nbr = env_list->two_point;
	if (nbr < 0)
		nbr = 0;
	pwd = get_env(env_list, "PWD");
	if (!pwd)
	{
		printf("Error: PWD not found in environment variables.\n");
		return (1);
	}
	printf("%s", pwd);
	i = 0;
	while (i < nbr)
	{
		printf("/..");
		i++;
	}
	printf("\n");
	return (0);
}

int	builtin_pwd(t_parser *parser, t_env *env)
{
	(void)parser;
	char	cwd[1024];

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

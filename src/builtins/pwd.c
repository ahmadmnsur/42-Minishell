

#include "../../minishell.h"

int ft_cmp(const char *s1, const char *s2)
{
    size_t i;

    i = 0;
    while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

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

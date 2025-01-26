
#include "../../minishell.h"

int	get_cmd_len(t_lexer *lexer)
{
	int	len;

	if (!lexer)
		return (0);
	len = 0;
	while (lexer && lexer->token == TOKEN_WORD)
	{
		len++;
		lexer = lexer->next;
	}
	return (len);
}

char	**get_cmd(t_lexer *lexer)
{
	char	**cmd;
	int		len;
	int		i;

	len = get_cmd_len(lexer);
	if (len == 0 || !lexer)
		return (NULL);
	cmd = (char **)malloc(sizeof(char *) * (len + 1));
	if (!cmd)
		return (NULL);
	i = 0;
	while (lexer && lexer->token == TOKEN_WORD)
	{
		cmd[i] = ft_strdup(lexer->str);
		lexer = lexer->next;
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}

int is_numeric(const char *str)
{
    if (!str || !*str)
        return (0);
    for (int i = 0; str[i]; i++)
        if (!ft_isdigit(str[i]))
            return (0);
    return (1);
}

int builtin_exit(t_parser *parser, t_env *env)
{
    (void)env;
    char **arg;
    int exit_status;
    int i;

    i = 0;

    arg = get_cmd(parser->tokens);
    if (!arg || !arg[0])
        return (0);
    if (!arg[1])
        exit(0);
    if (is_numeric(arg[1]))
    {
        if (arg[2])
        {
            printf("exit: too many arguments\n");
            for (int i = 0; arg[i]; i++)
                free(arg[i]);
            free(arg);
            return (1);
        }
        exit_status = ft_atoi(arg[1]);
    }
    else
    {
        printf("exit: %s: numeric argument required\n", arg[1]);
        exit_status = 2;
    }
    while(arg[i])
    {
        free(arg[i]);
        i++;
    }
    free(arg);
    exit(exit_status);
}

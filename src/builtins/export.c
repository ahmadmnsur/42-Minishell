

#include "../../minishell.h"

char **export_split(const char *str, const char *delimiter)
{
    char **result = malloc(5 * sizeof(char *));
    if (!result)
        return NULL;
    size_t delim_len = strlen(delimiter);
    char *delim_pos = strstr(str, delimiter);
    if (!delim_pos)
    {
        result[0] = NULL;
        result[1] = strdup(str);
        result[2] = NULL;
        result[3] = NULL;
        result[4] = NULL;
        return result;
    }
    result[0] = NULL;
    result[1] = strndup(str, delim_pos - str);
    result[2] = strndup(delimiter, delim_len);
    result[3] = strdup(delim_pos + delim_len);
    result[4] = NULL;
    return result;
}

int builtin_export(t_parser *parser, t_env *env)
{
    int i;
    char **arg = get_cmd(parser->tokens);
    i = 0;
    if (!arg[1])
    {
        print_full_export_list(env);
        return 0;
    }
    char **argcomment = export_split(arg[1], "=");
    if (!argcomment || !argcomment[1])
    {
        printf("export: Invalid input\n");
        if (argcomment)
            free_split_array(argcomment);
        return 1;
    }
    while (argcomment[1][i])
    {
        if ((i == 0 && ft_isdigit(argcomment[1][i])) || 
            (!ft_isalnum(argcomment[1][i]) && argcomment[1][i] != '_'))
        {
            printf("export: `%s`: not a valid identifier\n", argcomment[1]);
            free_split_array(argcomment);
            return 1;
        }
        i++;
    }
    t_env *current = env;
    int key_exists = 0;
    while (current)
    {
        if (ft_cmp(current->key, argcomment[1]) == 0)
        {
            key_exists = 1;
            break;
        }
        current = current->next;
    }
    if (!argcomment[2])
    {
        if (!key_exists)
            add_env_var(&env, argcomment[1], NULL);
    }
    else if (argcomment[2] && !argcomment[3])
        update_env_var(&env, argcomment[1], "");
    else if (argcomment[2] && argcomment[3])
        update_env_var(&env, argcomment[1], argcomment[3]);
    free_split_array(argcomment);
    return 0;
}

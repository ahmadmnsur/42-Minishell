




#include "../../minishell.h"

int get_env_len(t_env *env)
{
    int len;

    len = 0;
    while (env)
    {
        len++;
        env = env->next;
    }
    return (len);
}
char    **copy_envp_to_execve(t_env *env)
{
    char    **res;
    int     env_len;
    int     i;

    env_len = get_env_len(env);
    res = (char **)malloc(sizeof(char *) * (env_len + 1));
    if (!res)
        return (NULL);
    i = 0;
    while (env)
    {
        res[i] = ft_strjoin(ft_strdup(env->key), "=");
        res[i] = ft_strjoin(res[i], env->value);
        i++;
        env = env->next;
    }
    res[i] = NULL;
    return (res);
}
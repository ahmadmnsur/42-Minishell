

#include "../../minishell.h"

int cd_home(t_env *env)
{
    char *home = get_env(env, "HOME");
    char *pwd = get_env(env, "PWD");

    if (!home) {
        fprintf(stderr, "cd: HOME not set\n");
        return (1);
    }
    if (chdir(home) != 0) {
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

int cd_oldpwd(t_env *env)
{
    char *pwd = get_env(env, "PWD");
    char *oldpwd = get_env(env, "OLDPWD");

    if (!oldpwd) {
        fprintf(stderr, "cd: OLDPWD not set\n");
        return (1);
    }
    if (chdir(oldpwd) != 0) {
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

int cd_path(const char *path, t_env *env)
{
    char *current_pwd = get_env(env, "PWD");
    char *new_pwd;

    if (chdir(path) != 0) {
        perror("cd");
		env->two_point += 1;
        free(current_pwd);
        return (1);
    }
    new_pwd = getcwd(NULL, 0);
    if (!new_pwd) {
		env->two_point += 1;
        perror("cd: error getting current directory");
        free(current_pwd);
        return (1);
    }
    set_env(&env, "OLDPWD", current_pwd, 0);
    set_env(&env, "PWD", new_pwd, 0);
    free(new_pwd);
    free(current_pwd);
    return (0);
}

int builtin_cd(t_parser *parser, t_env *env)
{
    char **arg = get_cmd(parser->tokens->next);

    if (!arg || !arg[0]) {
        return cd_home(env);
    }
    if (arg[1]) {
        fprintf(stderr, "cd: too many arguments\n");
        return (1);
    }
    if (ft_cmp(arg[0], "~") == 0) {
        return cd_home(env);
    } else if (ft_cmp(arg[0], "-") == 0) {
        return cd_oldpwd(env);
    } else {
        return cd_path(arg[0], env);
    }
}

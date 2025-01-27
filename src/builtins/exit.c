/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:01:08 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/27 11:01:08 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

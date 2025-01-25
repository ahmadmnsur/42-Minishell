/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:00:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 16:00:51 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int builtin_env(t_parser *parser, t_env *env)
{
    (void)parser;
    t_env *tmp;
    if(!env)
    {
        fprintf(stderr, "env: No environment variables to display\n");
        return(0);
    }
    tmp = env;
    while(tmp != NULL)
    {
        if(tmp->hidden == 0)
			printf("%s=%s\n", tmp->key, tmp->value);
        tmp=tmp->next;
    }
    return(0);
}
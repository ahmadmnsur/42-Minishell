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

t_quote_type get_quote_type(char quote)
{
    if (quote == '\'')
        return (SINGLE_QUOTES);
    return (DOUBLE_QUOTES);
}


int get_quote_length(const char *str, int i, char quote)
{
    int len;

    len = 1;
    while (str[i + len] && str[i + len] != quote)
        len++;
    if (str[i + len] != quote)
        return ((int) ft_strlen(str) - i - 1);
    return (len);
}
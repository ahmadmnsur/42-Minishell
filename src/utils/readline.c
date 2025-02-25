/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:33:04 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 14:33:04 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// char	*minishell_loop(int status)
// {
// 	char	*prompt;
// 	char	*last_status;
// 	char	*readline_;

// 	if (status == 0)
// 	{
// 		prompt = ft_strdup("\001\033[32m\002minishell\001\033[0m\002\001\033[34m\002[0]\001\033[0m\002$ ");
// 		readline_ = readline(prompt);
// 		free(prompt);
// 	}
// 	else
// 	{
// 		prompt = ft_strdup("\001\033[32m\002minishell\001\033[0m\002");
// 		prompt = ft_strjoin(prompt, "\001\033[31m\002[");
// 		last_status = ft_itoa(status);
// 		prompt = ft_strjoin(prompt, last_status);
// 		free(last_status);
// 		prompt = ft_strjoin(prompt, "]\001\033[0m\002$ ");
// 		readline_ = readline(prompt);
// 		free(prompt);
// 	}
// 	return (readline_);
// }

char	*minishell_loop(int status)
{
	(void)status;
	return (readline("minishell>>"));
}

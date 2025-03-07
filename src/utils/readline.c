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

char	*minishell_loop(int status)
{
	(void)status;
	return (readline("4️⃣ 2️⃣  minishell  ➤  "));
}
